/**
 * @file config.js
 * @description This file contains the base configuration for the application's API services.
 *              It defines the BASE_URL used to interact with the backend server. This ensures
 *              consistency and ease of configuring API endpoints across the application.
 * @author Vladyslav Yeroma (xyerom00)
 */

/**
 * The base URL for all API requests in the application.
 * Adjust this value based on the backend server's deployment environment:
 * - Development: 'http://localhost:8081'
 * - Production: Replace with the actual production server URL.
 */
export const BASE_URL = 'http://localhost:8081'; // Base URL for the backend API
