#include "AIService.h"
#include <QTimer>
#include <QDateTime>
#include <QDebug>

/**
 * Mock AI Service Implementation
 * Simulates AI responses for development and testing
 */
class MockAIService : public AIService {
    Q_OBJECT

public:
    explicit MockAIService(QObject *parent = nullptr);

    void sendMessage(const QString &message) override;
    void sendMessageWithContext(const QString &message, const QStringList &context) override;
    void clearContext() override;
    
    bool isConfigured() const override { return true; }
    bool isConnected() const override { return m_isConnected; }
    AIProvider getProvider() const override { return AIProvider::Mock; }
    QString getModelName() const override { return m_modelName; }
    
    void setApiKey(const QString &apiKey) override { m_apiKey = apiKey; }
    void setModel(const QString &modelName) override { m_modelName = modelName; }
    void setMaxTokens(int maxTokens) override { m_maxTokens = maxTokens; }
    void setTemperature(double temperature) override { m_temperature = temperature; }
    
    QString getLastError() const override { return m_lastError; }
    int getUsageCount() const override { return m_usageCount; }
    void resetUsageCount() override { m_usageCount = 0; }

private slots:
    void simulateResponse();

private:
    QString generateMockResponse(const QString &input);
    QStringList m_conversationContext;
    QString m_pendingMessage;
    QTimer *m_responseTimer;
};

// AIService base class implementation
AIService::AIService(QObject *parent) 
    : QObject(parent)
    , m_modelName("mock-model-v1")
{
}

// MockAIService implementation
MockAIService::MockAIService(QObject *parent)
    : AIService(parent)
    , m_responseTimer(new QTimer(this))
{
    m_isConnected = true;
    m_modelName = "MockAI-v1.0";
    
    m_responseTimer->setSingleShot(true);
    connect(m_responseTimer, &QTimer::timeout, this, &MockAIService::simulateResponse);
}

void MockAIService::sendMessage(const QString &message) {
    if (message.trimmed().isEmpty()) {
        AIResponse response;
        response.status = ResponseStatus::Error;
        response.errorMessage = "Empty message provided";
        emit responseReceived(response);
        return;
    }
    
    m_pendingMessage = message;
    m_usageCount++;
    
    // Simulate network delay (500-2000ms)
    int delay = 500 + (qrand() % 1500);
    m_responseTimer->start(delay);
}

void MockAIService::sendMessageWithContext(const QString &message, const QStringList &context) {
    m_conversationContext = context;
    sendMessage(message);
}

void MockAIService::clearContext() {
    m_conversationContext.clear();
}

void MockAIService::simulateResponse() {
    AIResponse response;
    response.status = ResponseStatus::Success;
    response.content = generateMockResponse(m_pendingMessage);
    response.tokensUsed = m_pendingMessage.length() + response.content.length();
    response.responseTimeMs = 500 + (qrand() % 1500);
    
    // Add to conversation context
    m_conversationContext.append("User: " + m_pendingMessage);
    m_conversationContext.append("AI: " + response.content);
    
    // Keep context manageable (last 10 exchanges)
    while (m_conversationContext.size() > 20) {
        m_conversationContext.removeFirst();
    }
    
    emit responseReceived(response);
}

QString MockAIService::generateMockResponse(const QString &input) {
    QString lower = input.toLower();
    
    // Predefined responses for common queries
    if (lower.contains("hello") || lower.contains("hi")) {
        return "Hello! I'm a mock AI assistant integrated into NeuroShell. How can I help you today?";
    }
    
    if (lower.contains("help") || lower.contains("what can you do")) {
        return "I'm a mock AI service for development purposes. In the future, I'll be replaced with a real AI service that can:\n"
               "- Answer questions\n"
               "- Help with programming tasks\n"
               "- Provide explanations\n"
               "- Assist with terminal operations\n"
               "- And much more!";
    }
    
    if (lower.contains("weather")) {
        return "I'm a mock AI, so I can't check real weather data. But I can tell you it's always sunny in the world of mock responses! ☀️";
    }
    
    if (lower.contains("time") || lower.contains("date")) {
        return QString("According to my mock clock, it's %1. (This is a simulated response)")
                      .arg(QDateTime::currentDateTime().toString());
    }
    
    if (lower.contains("code") || lower.contains("program")) {
        return "I'd love to help you with coding! In the future, I'll be able to:\n"
               "- Write code snippets\n"
               "- Debug programs\n"
               "- Explain algorithms\n"
               "- Suggest best practices\n"
               "For now, I'm just a mock service preparing for that functionality.";
    }
    
    if (lower.contains("neuroshell") || lower.contains("terminal")) {
        return "NeuroShell is a great terminal application! I'm designed to be integrated into it to provide AI assistance. "
               "Once fully implemented, I'll be able to help you with terminal commands, explain outputs, and assist with various tasks.";
    }
    
    // Generic responses
    QStringList genericResponses = {
        QString("You said: '%1'. That's interesting! I'm a mock AI, so I'll give you a generic response for now.").arg(input),
        QString("Thanks for your message about '%1'. As a mock AI, I'm learning to provide better responses!").arg(input.left(20) + "..."),
        "That's a great question! When I'm fully implemented with a real AI service, I'll be able to provide much more helpful responses.",
        "I appreciate your input! Currently, I'm a mock service designed to test the AI integration in NeuroShell.",
        QString("Interesting point about '%1'. I'm looking forward to when I can provide real AI assistance!").arg(input.left(15) + "...")
    };
    
    return genericResponses[qrand() % genericResponses.size()];
}

// Factory implementation
AIService* AIServiceFactory::createService(AIService::AIProvider provider, QObject *parent) {
    switch (provider) {
        case AIService::AIProvider::Mock:
            return new MockAIService(parent);
        case AIService::AIProvider::OpenAI:
            // TODO: Implement OpenAIService
            qDebug() << "OpenAI service not yet implemented, falling back to Mock";
            return new MockAIService(parent);
        case AIService::AIProvider::Anthropic:
            // TODO: Implement AnthropicService  
            qDebug() << "Anthropic service not yet implemented, falling back to Mock";
            return new MockAIService(parent);
        case AIService::AIProvider::Local:
            // TODO: Implement LocalAIService
            qDebug() << "Local AI service not yet implemented, falling back to Mock";
            return new MockAIService(parent);
        case AIService::AIProvider::Custom:
            // TODO: Implement CustomAIService
            qDebug() << "Custom AI service not yet implemented, falling back to Mock";
            return new MockAIService(parent);
        default:
            return new MockAIService(parent);
    }
}

QStringList AIServiceFactory::getAvailableProviders() {
    return {"Mock", "OpenAI (TODO)", "Anthropic (TODO)", "Local (TODO)", "Custom (TODO)"};
}

QString AIServiceFactory::getProviderName(AIService::AIProvider provider) {
    switch (provider) {
        case AIService::AIProvider::Mock: return "Mock AI Service";
        case AIService::AIProvider::OpenAI: return "OpenAI GPT";
        case AIService::AIProvider::Anthropic: return "Anthropic Claude";
        case AIService::AIProvider::Local: return "Local AI Model";
        case AIService::AIProvider::Custom: return "Custom AI Service";
        default: return "Unknown Provider";
    }
}

#include "AIService.moc"