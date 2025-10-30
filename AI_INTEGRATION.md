# AI Integration for NeuroShell Terminal

This document describes the AI chatbot integration structure implemented in NeuroShell.

## Overview

The AI system is designed with a modular architecture that allows easy integration of different AI providers (OpenAI, Anthropic, local models, etc.) through a common interface.

## Architecture

### Core Components

1. **AIService** (Abstract Base Class)
   - Defines the interface for all AI providers
   - Handles common functionality and state management
   - Emits signals for responses and errors

2. **MockAIService** (Development Implementation)
   - Simulates AI responses for testing and development
   - Provides realistic delays and token usage simulation
   - Includes conversation context management

3. **AIServiceFactory**
   - Factory pattern for creating AI service instances
   - Centralized provider management
   - Easy switching between different AI providers

### Current Status

**✅ Implemented:**
- Base AIService interface
- MockAIService for development/testing
- Terminal integration with `ai` and `chat` commands
- Response formatting and error handling
- Factory pattern for service creation

**🚧 Planned/TODO:**
- OpenAI GPT integration
- Anthropic Claude integration  
- Local AI model support (Ollama, etc.)
- Custom API endpoint support
- Configuration management (API keys, etc.)
- Response streaming
- File attachment support
- Advanced context management

## Usage

### Terminal Commands

```bash
# Basic AI interaction
ai hello
chat what can you do?

# AI status and management
ai status          # Show service info
ai clear           # Clear conversation context

# Help
ai                 # Show usage help
chat               # Same as ai
```

### Example Session

```
cli >> ai hello
Sending to AI: hello
Waiting for response...
AI Response:
─────────────
  Hello! I'm a mock AI assistant integrated into NeuroShell. How can I help you today?
─────────────

cli >> chat what can you do?
Sending to AI: what can you do?
Waiting for response...
AI Response:
─────────────
  I'm a mock AI service for development purposes. In the future, I'll be replaced with a real AI service that can:
  - Answer questions
  - Help with programming tasks
  - Provide explanations
  - Assist with terminal operations
  - And much more!
─────────────

cli >> ai status
AI Service Status:
  Provider: Mock AI Service
  Model: MockAI-v1.0
  Connected: Yes
  Configured: Yes
  Usage Count: 2
```

## Implementation Details

### File Structure

- `AIService.h` - Abstract interface and factory declarations
- `AIService.cpp` - Mock implementation and factory
- `TerminalWindow.h/.cpp` - Terminal integration

### Key Features

**Provider Abstraction:**
```cpp
enum class AIProvider {
    Mock,           // For testing and development
    OpenAI,         // OpenAI GPT models
    Anthropic,      // Anthropic Claude models
    Local,          // Local/offline models
    Custom          // Custom API endpoints
};
```

**Response Structure:**
```cpp
struct AIResponse {
    ResponseStatus status;
    QString content;
    QString errorMessage;
    int tokensUsed = 0;
    qint64 responseTimeMs = 0;
};
```

**Async Communication:**
```cpp
// Send message (async)
m_aiService->sendMessage("Hello AI");

// Handle response (signal/slot)
connect(m_aiService, &AIService::responseReceived, this, 
        &TerminalWindow::handleAIResponse);
```

### MockAIService Behavior

The mock service provides:
- Realistic response delays (500-2000ms)
- Context-aware responses for common queries
- Token usage simulation
- Conversation history tracking
- Error simulation capabilities

## Future Integration Guide

### Adding a Real AI Provider

1. **Create Provider Class:**
```cpp
class OpenAIService : public AIService {
    Q_OBJECT
public:
    OpenAIService(QObject *parent = nullptr);
    void sendMessage(const QString &message) override;
    // ... implement other virtual methods
};
```

2. **Update Factory:**
```cpp
AIService* AIServiceFactory::createService(AIProvider provider, QObject *parent) {
    switch (provider) {
        case AIProvider::OpenAI:
            return new OpenAIService(parent);
        // ...
    }
}
```

3. **Add Configuration:**
- API key management
- Model selection
- Parameter tuning (temperature, max tokens, etc.)

### Configuration Management

Future implementation should include:
- QSettings-based configuration storage
- Environment variable support
- Runtime configuration changes
- Multiple provider profiles

### Advanced Features

**Streaming Responses:**
```cpp
signals:
    void responseChunkReceived(const QString &chunk);
    void responseCompleted();
```

**File Attachments:**
```cpp
void sendMessageWithFiles(const QString &message, 
                         const QStringList &filePaths);
```

**Function Calling:**
```cpp
struct AIFunction {
    QString name;
    QString description;
    QJsonObject parameters;
};
```

## Security Considerations

- Store API keys securely (encrypted settings)
- Validate all inputs before sending to AI
- Implement rate limiting
- Handle sensitive data appropriately
- Provide user control over data sharing

## Testing

The mock service enables comprehensive testing:
- UI integration testing
- Response handling verification
- Error scenario simulation
- Performance characteristics testing

## Dependencies

Current: Qt6 Widgets (existing)
Future: 
- Qt6 Network (HTTP requests)
- Qt6 WebSockets (streaming)
- OpenSSL (security)

---

*This integration provides a solid foundation for AI functionality while maintaining clean separation of concerns and extensibility for future AI providers.*