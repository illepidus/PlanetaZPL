#include "zplhighlighter.h"

ZPLHighlighter::ZPLHighlighter(QTextDocument *parent) : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    fdfs_format.setFontWeight(QFont::Bold);
    fdfs_format.setForeground(Qt::red);
    rule.pattern = QRegularExpression(QStringLiteral("\\^FD.*\\^FS"));
    rule.format = fdfs_format;
    highlightingRules.append(rule);

    command_format.setFontWeight(QFont::Normal);
    command_format.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression(QStringLiteral("\\^.."));
    rule.format = command_format;
    highlightingRules.append(rule);

    command_format.setFontWeight(QFont::Bold);
    command_format.setForeground(Qt::blue);
    rule.pattern = QRegularExpression(QStringLiteral("\\>[\\d;:=<]"));
    rule.format = command_format;
    highlightingRules.append(rule);
}

void ZPLHighlighter::highlightBlock(const QString &text)
{
    for (const HighlightingRule &rule : qAsConst(highlightingRules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
    setCurrentBlockState(0);
}
