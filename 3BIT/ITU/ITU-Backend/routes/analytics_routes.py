from flask import Blueprint, request, jsonify
from services.analytics_service import *

analytics_bp = Blueprint('analytics', __name__)

#TODO: api/analytics
# GET /api/analytics/overview?start_date=&end_date=&providers=&calendars=
# GET /api/analytics/availability?start_date=&end_date=&providers=&calendars=
# GET /api/analytics/trends?start_date=&end_date=&metric=
# GET /api/analytics/providers/performance?start_date=&end_date=&p
# GET /api/analytics/services/performance?start_date=&end_date=&services=
@analytics_bp.route('/analytics/provider/total_bookings/<int:provider_id>', methods=['GET'])
def get_total_bookings_by_provider(provider_id):
    # Extract query parameters
    start = request.args.get('start')
    end = request.args.get('end')

    if start and end:
        # Parse providers to a list of integers
        total_bookings = get_total_bookings_by_provider_criteria(start, end, provider_id)
    else:
        # Return all appointments if no filters are provided
        total_bookings = get_total_bookings_by_provider(provider_id)

    return jsonify(total_bookings), 200