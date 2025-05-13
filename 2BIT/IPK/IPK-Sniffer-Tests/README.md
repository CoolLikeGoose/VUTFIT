# Basic Python tests for IPK 2nd project - Sniffer

Basic tests sending different types of messages and testing correct filtering of these messages.

## How to run it
1. Create virtual env `python3 -m venv env`
2. Activate it by `. env/bin/activate`
> Note: this must be done every time you use new terminal
3. `pip install -r requirements.txt`
4. Edit binary path in `ipkSnifferTests.py`
5. Simply run with `sudo python3 ipkSnifferTests.py`

Specific test can be run by `sudo python3 -m unittest ipkSnifferTests.SnifferTests.test_name`, where `test_name` is the name of the test

Feel free to edit and do what you like with these tests.

