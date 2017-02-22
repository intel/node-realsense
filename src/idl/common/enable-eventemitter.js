////////////////////////////////////////////////////////////
var EventEmitter = require('events').EventEmitter;
function inherits(target, source) {
  for (var k in source.prototype) {
    target.prototype[k] = source.prototype[k];
  }
}

inherits(Instance, EventEmitter);
