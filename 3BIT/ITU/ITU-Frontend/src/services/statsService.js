import { BASE_URL } from './config';

export const statsService = {
    getKeyMetrics: async () => {
        try {
            const response = await fetch(`${BASE_URL}/analytics/overview?start_date=&end_date=&providers=&calendars=`);
            if (!response.ok) {
                throw new Error(`Failed to fetch overview calendars`);
            }
            return await response.json();
        } catch (error) {
            console.error("Error fetching overview calendars :", error);
            throw error;
        }
    },
    getAvailabilityPercentage: async () => {
        try {
            const response = await fetch(`${BASE_URL}/analytics/availability?start_date=&end_date=&providers=&calendars=`);
            if (!response.ok) {
                throw new Error(`Failed to fetch availability percentage`);
            }
            return await response.json();
        } catch (error) {
            console.error("Error fetching availability percentage :", error);
            throw error;
        }
    },
    getBookingTrends: async () => {
        try {
            const response = await fetch(`${BASE_URL}/analytics/trends?start_date=&end_date=&metric=`);
            if (!response.ok) {
                throw new Error(`Failed to fetch booking trends`);
            }
            return await response.json();
        } catch (error) {
            console.error("Error fetching booking trends :", error);
            throw error;
        }
    },
    getAnnotationById: async (annotationId) => {
        try {
            const response = await fetch(`${BASE_URL}/annotations/${annotationId}`);
            if (!response.ok) {
                throw new Error(`Failed to fetch annotation with ID ${annotationId}`);
            }
            return await response.json();
        } catch (error) {
            console.error("Error fetching annotation data:", error);
            throw error;
        }
    },
    updateAnnotation: async (annotationId, data) => {
        const response = await fetch(`${BASE_URL}/annotations/${annotationId}`, {
            method: 'PUT',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(data),
        });
        if (!response.ok) {
            throw new Error('Failed to update annotation');
        }
        return await response.json();
    },
    addAnnotation: async (data) => {
        const response = await fetch(`${BASE_URL}/annotations/`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(data),
        });
        if (!response.ok) {
            throw new Error('Failed to update annotation');
        }
        return await response.json();
    },
    deleteAnnotation: async (annotationId) => {
        const response = await fetch(`${BASE_URL}/annotations/${annotationId}`, {
            method: 'DELETE',
        });
        if (!response.ok) {
            throw new Error('Failed to delete annotation');
        }
        return await response.json();
    },
    getThresholdById: async (thresholdId) => {
        try {
            const response = await fetch(`${BASE_URL}/thresholds/${thresholdId}`);
            if (!response.ok) {
                throw new Error(`Failed to fetch threshold with ID ${thresholdId}`);
            }
            return await response.json();
        } catch (error) {
            console.error("Error fetching threshold data:", error);
            throw error;
        }
    },
    updateThreshold: async (thresholdId, data) => {
        const response = await fetch(`${BASE_URL}/thresholds/${thresholdId}`, {
            method: 'PUT',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(data),
        });
        if (!response.ok) {
            throw new Error('Failed to update threshold');
        }
        return await response.json();
    },
    addThreshold: async (data) => {
        const response = await fetch(`${BASE_URL}/thresholds/`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(data),
        });
        if (!response.ok) {
            throw new Error('Failed to update threshold');
        }
        return await response.json();
    },
    deleteThreshold: async (thresholdId) => {
        const response = await fetch(`${BASE_URL}/thresholds/${thresholdId}`, {
            method: 'DELETE',
        });
        if (!response.ok) {
            throw new Error('Failed to delete threshold');
        }
        return await response.json();
    },

};