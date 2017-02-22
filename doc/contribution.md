# Contribution
Go to [Setup Environment](./setup_environment.md) first to ensure all your hardware and software are ready.

## Coding style guideline
We're following [Chromium coding style](https://chromium.googlesource.com/chromium/src/+/master/styleguide/styleguide.md) for different languages: [C++](https://chromium.googlesource.com/chromium/src/+/master/styleguide/c++/c++.md), [Python](https://google.github.io/styleguide/pyguide.html) and [JavaScript](https://google.github.io/styleguide/javascriptguide.xml).

### Install tools
* [depot_tools](https://www.chromium.org/developers/how-tos/install-depot-tools) need to be installed and added to `PATH` env.
* Re-use [widl-nan](http://github.com/01org/widl-nan) linter tool
```
cd src/tools && npm install
```

### Run tests
* Prepare the playback files by running `./src/common/tests/fetch_tests_data.sh <playback_url>`

### Run linter
Run `python ./tools/node_modules/widl-nan/tools/lint.py` before submitting your code.

## Commit message guideline
We use same [Chromium commit log guideline](https://www.chromium.org/developers/contributing-code) and [Github closing isses via commit messages](https://help.github.com/articles/closing-issues-via-commit-messages/). Use the following form:

```
Summary of change.

Longer description of change addressing as appropriate: why the change is made,
context if it is part of many changes, description of previous behavior and
newly introduced differences, etc.

Long lines should be wrapped to 80 columns for easier log message viewing in
terminals.

Fixes #123456
```

## If you need release a build, follow [How to Release](./howto_release.md)
