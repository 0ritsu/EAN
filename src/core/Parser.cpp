#include "Parser.h"

ParseResult Parser::parse(const QString &text) {
    ParseResult result;
    if (text.trimmed().isEmpty()) {
        result.ok = false;
        result.error = "Puste wejście.";
        return result;
    }

    // TODO: pełny parser formatu A=..., b=...
    result.ok = true;
    result.raw = text;
    return result;
}
