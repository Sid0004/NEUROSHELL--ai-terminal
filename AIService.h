#pragma once

#include <QObject>
#include <QString>
#include <QStringList>

/**
 * Abstract interface for AI chatbot services
 * This provides a foundation for integrating various AI providers
 * (OpenAI, Anthropic, local models, etc.) in the future
 */
class AIService : public QObject {
    Q_OBJECT

public:
    enum class AIProvider {
        Mock,           // For testing and development
        OpenAI,         // OpenAI GPT models
        Anthropic,      // Anthropic Claude models
        Local,          // Local/offline models
        Custom          // Custom API endpoints
    };

    enum class ResponseStatus {
        Success,
        Error,
        Timeout,
        NetworkError,
        AuthError,
        RateLimited
    };

    struct AIResponse {
        ResponseStatus status;
        QString content;
        QString errorMessage;
        int tokensUsed = 0;
        qint64 responseTimeMs = 0;
    };

    explicit AIService(QObject *parent = nullptr);
    virtual ~AIService() = default;

    // Core AI functionality
    virtual void sendMessage(const QString &message) = 0;
    virtual void sendMessageWithContext(const QString &message, const QStringList &context) = 0;
    virtual void clearContext() = 0;
    
    // Configuration
    virtual bool isConfigured() const = 0;
    virtual bool isConnected() const = 0;
    virtual AIProvider getProvider() const = 0;
    virtual QString getModelName() const = 0;
    
    // Settings
    virtual void setApiKey(const QString &apiKey) = 0;
    virtual void setModel(const QString &modelName) = 0;
    virtual void setMaxTokens(int maxTokens) = 0;
    virtual void setTemperature(double temperature) = 0;
    
    // Status and diagnostics
    virtual QString getLastError() const = 0;
    virtual int getUsageCount() const = 0;
    virtual void resetUsageCount() = 0;

signals:
    void responseReceived(const AIResponse &response);
    void errorOccurred(const QString &error);
    void connectionStatusChanged(bool connected);

protected:
    QString m_apiKey;
    QString m_modelName;
    int m_maxTokens = 1000;
    double m_temperature = 0.7;
    QString m_lastError;
    int m_usageCount = 0;
    bool m_isConnected = false;
};

/**
 * Factory class for creating AI service instances
 */
class AIServiceFactory {
public:
    static AIService* createService(AIService::AIProvider provider, QObject *parent = nullptr);
    static QStringList getAvailableProviders();
    static QString getProviderName(AIService::AIProvider provider);
};