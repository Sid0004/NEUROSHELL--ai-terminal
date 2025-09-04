#include "TerminalWindow.h"

#include <QTextEdit>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFontDatabase>
#include <QPalette>
#include <QDate>
#include <QTime>
#include <QKeyEvent>
#include <QSysInfo>

TerminalWindow::TerminalWindow(QWidget *parent)
    : QWidget(parent), m_output(new QTextEdit(this)), m_input(new QLineEdit(this)) {
    setWindowTitle("NeuroShell Terminal");
    // Open with a large default size
    resize(1200, 800);
    setMinimumSize(800, 600);

    // Layout
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setSpacing(6);

    m_output->setReadOnly(true);
    m_output->setUndoRedoEnabled(false);
    m_output->setAcceptRichText(false);

    // Monospace font
    QFont mono = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    mono.setPointSize(11);
    m_output->setFont(mono);
    m_input->setFont(mono);

    // Green on black theme
    QPalette pal;
    pal.setColor(QPalette::Base, QColor(0, 0, 0));
    pal.setColor(QPalette::Text, QColor(0, 255, 0));
    pal.setColor(QPalette::Window, QColor(0, 0, 0));
    pal.setColor(QPalette::WindowText, QColor(0, 255, 0));
    pal.setColor(QPalette::PlaceholderText, QColor(0, 180, 0));
    m_output->setPalette(pal);
    m_input->setPalette(pal);
    setPalette(pal);

    layout->addWidget(m_output);
    // prompt + input row
    auto *row = new QHBoxLayout();
    row->setSpacing(8);
    m_prompt = new QLabel("cli >>", this);
    m_prompt->setFont(mono);
    m_prompt->setPalette(pal);
    row->addWidget(m_prompt, 0);
    row->addWidget(m_input, 1);
    layout->addLayout(row);

    // ASCII logo/banner
    appendOutput(" _   _                           _____ _          _ _ ");
    appendOutput("| \n| | ___ _ __ _ __ ___   ___  / ____| |        | | |");
    appendOutput("| | | |/ _ \\ '__| '_ ` _ \\ / _ \\ | (___ | |__   ___| | | ___ _ __");
    appendOutput("| |_| |  __/ |  | | | | | |  __/  \\___ \\| '_ \\ / _ \\ | |/ _ \\ '__|");
    appendOutput("|  _  |\\___|_|  |_| |_| |_|\\___|  ____) | | | |  __/ | |  __/ |");
    appendOutput("|_| |_|                         |_____/|_| |_|\\___|_|_|\\___|_|");
    appendOutput("");
    appendOutput("Welcome to NeuroShell v0.1");
    appendOutput("Type 'help' to see available commands.");
    appendOutput("");
    appendOutput(getPrompt());

    // History navigation with Up/Down
    m_input->installEventFilter(this);

    connect(m_input, &QLineEdit::returnPressed, this, &TerminalWindow::handleReturnPressed);
}

void TerminalWindow::appendOutput(const QString &text) {
    m_output->append(text);
}

void TerminalWindow::printHelp() {
    appendOutput("Available commands:");
    appendOutput("  help      - Show this help message");
    appendOutput("  clear, cls- Clear the screen");
    appendOutput("  aboutme   - Show project info");
    appendOutput("  echo TEXT - Print TEXT");
    appendOutput("  date      - Show current date");
    appendOutput("  time      - Show current time");
    appendOutput("  whoami    - Show current user");
    appendOutput("  version   - Show app/Qt version");
    appendOutput("  history   - Show recent commands");
    appendOutput("  exit      - Quit NeuroShell");
}

void TerminalWindow::aboutMe() {
    appendOutput("NeuroShell: Terminal Project");
    appendOutput("Created by Siddhant Sharma");
}

void TerminalWindow::handleReturnPressed() {
    const QString command = m_input->text().trimmed();
    m_input->clear();

    if (command.isEmpty()) {
        return;
    }

    // replace last prompt line with command + echo
    m_output->moveCursor(QTextCursor::End);
    m_output->textCursor().deletePreviousChar(); // ensure proper newline state
    appendOutput(getPrompt() + command);
    historyAdd(command);
    runCommand(command);
    appendOutput("");
    appendOutput(getPrompt());
}

void TerminalWindow::runCommand(const QString &commandLine) {
    const QString cmd = commandLine.section(' ', 0, 0).toLower();
    const QString args = commandLine.mid(cmd.size()).trimmed();

    if (cmd == "help") {
        printHelp();
        return;
    }
    if (cmd == "clear" || cmd == "cls") {
        m_output->clear();
        return;
    }
    if (cmd == "aboutme") {
        aboutMe();
        return;
    }
    if (cmd == "echo") {
        appendOutput(args);
        return;
    }
    if (cmd == "date") {
        appendOutput(QDate::currentDate().toString(Qt::ISODate));
        return;
    }
    if (cmd == "time") {
        appendOutput(QTime::currentTime().toString("HH:mm:ss"));
        return;
    }
    if (cmd == "whoami") {
        appendOutput(qEnvironmentVariable("USERNAME", qEnvironmentVariable("USER", "unknown")));
        return;
    }
    if (cmd == "version") {
        appendOutput(QString("NeuroShell v0.1 | Qt ") + QT_VERSION_STR + " | " + QSysInfo::prettyProductName());
        return;
    }
    if (cmd == "history") {
        for (int i = 0; i < m_history.size(); ++i) {
            appendOutput(QString::number(i + 1) + ": " + m_history.at(i));
        }
        return;
    }
    if (cmd == "exit") {
        close();
        return;
    }

    appendOutput(QString("Unknown command: ") + commandLine);
}

QString TerminalWindow::getPrompt() const {
    return "cli >> ";
}

void TerminalWindow::historyAdd(const QString &cmd) {
    if (!cmd.isEmpty() && (m_history.isEmpty() || m_history.last() != cmd)) {
        m_history.append(cmd);
    }
    m_historyIndex = -1;
}

void TerminalWindow::historyPrev() {
    if (m_history.isEmpty()) return;
    if (m_historyIndex == -1) m_historyIndex = m_history.size();
    if (m_historyIndex > 0) --m_historyIndex;
    m_input->setText(m_history.at(m_historyIndex));
}

void TerminalWindow::historyNext() {
    if (m_history.isEmpty()) return;
    if (m_historyIndex == -1) return;
    if (m_historyIndex < m_history.size() - 1) {
        ++m_historyIndex;
        m_input->setText(m_history.at(m_historyIndex));
    } else {
        m_historyIndex = -1;
        m_input->clear();
    }
}

bool TerminalWindow::eventFilter(QObject *watched, QEvent *event) {
    if (watched == m_input && event->type() == QEvent::KeyPress) {
        auto *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Up) {
            historyPrev();
            return true;
        }
        if (keyEvent->key() == Qt::Key_Down) {
            historyNext();
            return true;
        }
    }
    return QWidget::eventFilter(watched, event);
}


