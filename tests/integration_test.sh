#!/bin/bash

echo "Running integration tests..."

# Test GET /
response=$(curl -s http://localhost:8080/)
if [[ $response == *"Hello from CI/CD"* ]]; then
    echo "✓ GET / passed"
else
    echo "✗ GET / failed"
    exit 1
fi

# Test GET /health
response=$(curl -s http://localhost:8080/health)
if [[ $response == "OK" ]]; then
    echo "✓ GET /health passed"
else
    echo "✗ GET /health failed"
    exit 1
fi

# Test GET /metrics
response=$(curl -s http://localhost:8080/metrics)
if [[ $response == *"http_requests_total"* ]]; then
    echo "✓ GET /metrics passed"
else
    echo "✗ GET /metrics failed"
    exit 1
fi

echo "All integration tests passed!"
exit 0