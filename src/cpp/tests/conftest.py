def pytest_addoption(parser) -> None:
    parser.addoption('--binary', action='store')
    parser.addoption('--port', action='store')
