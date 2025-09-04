#pragma once

#include <QWidget>
#include <QStringList>

class QTextEdit;
class QLineEdit;
class QLabel;

class TerminalWindow : public QWidget {
    Q_OBJECT
public:
    explicit TerminalWindow(QWidget *parent = nullptr);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void handleReturnPressed();

private:
    void appendOutput(const QString &text);
    void printHelp();
    void aboutMe();
    void runCommand(const QString &commandLine);
    QString getPrompt() const;

    // History helpers
    void historyAdd(const QString &cmd);
    void historyPrev();
    void historyNext();

    QTextEdit *m_output;
    QLineEdit *m_input;
    QLabel *m_prompt;
    QStringList m_history;
    int m_historyIndex = -1; // -1 means not currently navigating
};


