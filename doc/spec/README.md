# Editorial Documentation

The specification is built using Bikeshed which is a pre-processor for spec documents. If you want to contribute to the spec, please make sure that they result in a specification that will build correctly.

1. Install [Bikeshed](https://github.com/tabatkins/bikeshed).
2. Edit the source file `*.bs` under the `bikeshed` folder.
3. Run bikeshed to generate a final spec document: `cd doc/spec; make`

**NOTE**:
1. DONOT edit the .html directly.
2. Bikeshed will inject its SHA1 on meta, thus the generated .html will be changed. Ideally we do not accept that kind of change with only meta line changed.
3. The detailed features of Bikeshed could refer to [Bikeshed Documentation](https://tabatkins.github.io/bikeshed).
