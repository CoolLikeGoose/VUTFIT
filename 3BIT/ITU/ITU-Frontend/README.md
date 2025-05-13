## Requirements

Before running this project, make sure you have the following tools installed:

- [Node.js](https://nodejs.org/en/) (v14 or later)
- [npm](https://www.npmjs.com/) (comes with Node.js)

## Installation

1. Install the project dependencies:

```bash
npm install
```

## Development Server

To start the development server and automatically open the application in your default browser:

```bash
npm start
```

This will launch the app at `http://localhost:8080`.

## Building the Project

To create a production-ready build of the project, run the following command:

```bash
npm run build
```

The build artifacts will be generated in the `deployment/` directory.

## Configuration

You can change the API base URL or other configuration values by editing the `src/services/config.js` file.

```javascript
export const BASE_URL = 'http://localhost:8081';
```