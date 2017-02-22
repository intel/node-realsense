# Workaround of node-gyp build issue
target=`ls build/*.mk`
alter=`ls build/*.mk`".new"
sed 's/widl\ FORCE_DO_CMD/widl/' -r $target > $alter
cp -f $alter $target

# Build current add-on
node-gyp build -j 8

# Lint tool -- Make your code great again by making lint happy again
if [ $? -eq 0 ]; then
	# Save current dir
	saved=`pwd`

	cd ../../ && python tools/node_modules/widl-nan/tools/lint.py --base=master

	# Test: to do, or not to do
	if [ $# -gt 0 ] && [ $1 == "test" ]; then
		cd $saved && mocha $2
	else
		echo "Note: to run test, type '$0 test' or '$0 test <file>'"
	fi
fi
