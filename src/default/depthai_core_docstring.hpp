// Default DOC define, which is nullptr aka no docstring
// This case covers when docstrings couldn't be generated
// Upon succesful mkdoc invoication, this file should not be included
#define DOC(...) ((const char*) nullptr)
