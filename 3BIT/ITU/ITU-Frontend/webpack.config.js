const path = require('path');

module.exports = {
    entry: './src/index.js', // Your entry point
    output: {
        path: path.resolve(__dirname, 'deployment'), // Output in the deployment folder
        filename: 'bundle.js', // The output JavaScript file
    },
    devServer: {
        static: {
            directory: path.join(__dirname, 'deployment'), // Serve content from the 'deployment' folder
        },
        compress: true, // Enable gzip compression for assets
        port: 8080, // Port for your server
        historyApiFallback: true, // Required for SPA routing to work
        open: true, // Automatically open the app in the default browser
    },
    module: {
        rules: [
            {
                test: /\.(js|jsx)$/, // Match .js and .jsx files
                exclude: /node_modules/,
                use: {
                    loader: 'babel-loader',
                },
            },
            {
                test: /\.css$/, // For handling CSS files
                use: ['style-loader', 'css-loader'],
            },
        ],
    },
    resolve: {
        extensions: ['.js', '.jsx'], // Resolve JavaScript and JSX files
    },
    mode: 'development', // Development mode
};
