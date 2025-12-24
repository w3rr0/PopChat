# PopChat

A lightweight desktop widget that provides instant access to local language models via Ollama engine. 

Designed to be unobtrusive and efficient, PopChat acts as a quick overlay for your AI interactions without cluttering your workspace. It brings the power of local LLMs to your fingertips with a single keystroke.

## Features

* **Instant Access:** Toggle the chat window globally with a shortcut (default: `Ctrl+Shift+K`)
* **Minimalist Design:** A compact, floating UI that stays out of your way
* **Local & Private:** Fully powered by your local [Ollama](https://ollama.com/) instance
* **Configurable:** Easily switch between models (e.g., Llama3, Mistral) via the settings menu)

## Getting Started

### Prerequisites

1.  **Ollama**: Ensure Ollama is installed (you can check by `ollama list`)
2.  **Model**: Pull a model if you haven't already (e.g., `ollama pull llama3`)

### Installation

PopChat is build with **C++** and **Qt6**.

Remember to provide the path to Qt library in .env file (look .env.example).

## Usage

1. Launch the application
2. Click the settings icon to select or change the active model
3. Type your prompt and hit enter
4. Press Ctrl+Shift+k to show or hide the widget

---

Distributed under the `MIT License`. See [LICENSE](https://github.com/w3rr0/PopChat/blob/master/LICENSE) for more information.
