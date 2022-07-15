#ifndef ZPLHIGHLIGHTER_H
#define ZPLHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>

class ZPLHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    ZPLHighlighter(QTextDocument *parent = 0);

protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QTextCharFormat fdfs_format;
    QTextCharFormat command_format;
};

#endif // ZPLHIGHLIGHTER_H
