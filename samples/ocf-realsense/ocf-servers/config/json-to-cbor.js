// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.
// limitations under the License.

module.exports = function( fileName, resourceUuid, noClobber ) {
// Create two ACEs from the given resources - one for plain text and one for encrypted
function generateACEList( resources ) {
	return [
		{subject: {'conntype': 'anon-clear'}, resources: resources, 'permission': 31}, // eslint-disable-line
		{subject: {'conntype': 'auth-crypt'}, resources: resources, 'permission': 31} // eslint-disable-line
	];
}

let _ = {
	mergeWith: require( 'lodash.mergewith' ),
	extend: require( 'lodash.assignin' ) // eslint-disable-line
};
let fs = require( 'fs' );
let path = require( 'path' );
let uuid = require( 'uuid' );
let spawnSync = require( 'child_process' ).spawnSync;
let configurationDirectory = require( 'iotivity-node/lib/configurationDirectory' );

let configuration;
let toolResult;
let toolPath;
let configPath;

let modulePath = path.dirname(require.resolve('iotivity-node'));
let installPrefix = path.join( modulePath, 'iotivity-installed' );
let deviceUuid = uuid.v4();

if ( fileName ) {
	fileName = path.normalize( fileName );
}

configPath = configurationDirectory.apply( configurationDirectory, fileName ? [fileName] : [] );

if ( fs.existsSync( path.join( configPath, 'oic_svr_db.dat' ) ) && noClobber ) {
	return;
}

// Load the configuration (if any) and add a permissive acl for the path ('/a/' + uuid)
configuration = _.mergeWith( {},

	// The boilerplate
	require( './security.boilerplate.json' ),

	// Special href containing the resource uuid
	{
		acl: {
			aclist2: generateACEList( resourceUuid ) // eslint-disable-line
		} // eslint-disable-line
	},

	// The device ID
	{
		pstat: {deviceuuid: deviceUuid, rowneruuid: deviceUuid},
		acl: {rowneruuid: deviceUuid},
		doxm: {deviceuuid: deviceUuid, deviceuuid: deviceUuid} // eslint-disable-line
	},

	// Function to merge aces
	function( objectValue, sourceValue ) {
		if ( Array.isArray( objectValue ) && Array.isArray( sourceValue ) ) {
			return objectValue.concat( sourceValue );
		}
	} );

configuration.acl.aclist2 = configuration.acl.aclist2.map( function( value, index ) {
	return _.extend( value, {aceid: index + 1} );
} );

toolPath = path.join( installPrefix, 'bin' );

fs.writeFileSync( path.join( configPath, 'oic_svr_db.json' ),
	JSON.stringify( configuration, null, '\t' ) );

toolResult = spawnSync( 'json2cbor', [
	path.join( configPath, 'oic_svr_db.json' ),
	path.join( configPath, 'oic_svr_db.dat' )], {
		env: _.extend( {}, process.env, {
			PATH: ( process.env.PATH || '' )
				.split( path.delimiter )
				.concat( [toolPath] )
				.join( path.delimiter ),
			LD_LIBRARY_PATH: path.join( installPrefix, 'lib' ),
			stdio: [process.stdin, process.stdout, process.stderr] // eslint-disable-line
		} ) // eslint-disable-line
	} );

if ( toolResult.status !== 0 ) {
	console.log( JSON.stringify( {info: true, message:
		'Warning: Failed to run json2cbor: ' + toolResult.stderr // eslint-disable-line
	} ) );
}
};
