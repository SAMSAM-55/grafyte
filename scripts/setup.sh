#!/bin/bash

echo "Creating Python virtual environment..."
python3 -m venv .venv

if [ $? -ne 0 ]; then
    echo "[ERROR] Failed to create virtual environment."
    exit 1
fi

echo "Activating virtual environment..."
source .venv/bin/activate

if [ $? -ne 0 ]; then
    echo "[ERROR] Failed to activate virtual environment."
    exit 1
fi

echo "Installing build-related dependencies..."
pip install build auditwheel

if [ $? -ne 0 ]; then
    echo "[ERROR] Failed to install dependencies."
    exit 1
fi

echo -e "\n[SUCCESS] Setup complete!"
echo "Virtual environment created and dependencies installed."
echo -e "\nTo activate the virtual environment, run:"
echo "source .venv/bin/activate\n"
