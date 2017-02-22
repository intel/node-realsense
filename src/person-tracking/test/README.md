# How to run unit test
## Prepare
```
npm install mocha -g
```

## Build addon from source
```
cd src/person-tracking
npm install
```

## Run unit test
+ run one single test
```
mocha test/BoundingBox2DInfo/test-boundingbox2dinfo-api-existence.js
```
+ run all api tests
```
mocha test/*/*.js
```
+ run all 'personTrackerOptions' tests
```
mocha test/*/*/*.js
```

# IDL:
the code is based on the ./idl/pt.widl currently.
