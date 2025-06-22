"""Django views for pyOxide admin interface."""

import json
from django.http import JsonResponse, HttpResponse
from django.shortcuts import render
from django.contrib.auth.models import User
from django.views.decorators.csrf import csrf_exempt
from django.views.decorators.http import require_http_methods
from typing import Any, Dict


def api_info(request: Any) -> JsonResponse:
    """Return Django API information."""
    info = {
        "name": "pyOxide Django Admin API",
        "version": "1.0.0",
        "description": "Embedded Django admin interface for pyOxide",
        "endpoints": [
            "/admin/ - Django admin interface",
            "/api/django/ - This API info",
            "/api/django/data/ - Data management API",
            "/api/django/users/ - User management API",
            "/dashboard/ - Admin dashboard",
        ],
        "features": [
            "User management",
            "Data administration",
            "Admin interface",
            "REST API endpoints",
        ],
    }
    return JsonResponse(info)


@csrf_exempt
@require_http_methods(["GET", "POST"])
def data_management(request: Any) -> JsonResponse:
    """Handle data management operations."""
    if request.method == "GET":
        # Return some sample data
        data = {
            "total_users": User.objects.count(),
            "sample_data": [
                {"id": 1, "name": "Sample Item 1", "status": "active"},
                {"id": 2, "name": "Sample Item 2", "status": "inactive"},
            ],
            "message": "Data retrieved successfully",
        }
        return JsonResponse(data)

    elif request.method == "POST":
        try:
            data = json.loads(request.body)
            # Process the data (this is just a demo)
            response_data = {
                "message": "Data processed successfully",
                "received_data": data,
                "status": "success",
            }
            return JsonResponse(response_data)
        except json.JSONDecodeError:
            return JsonResponse(
                {"error": "Invalid JSON data", "status": "error"}, status=400
            )


def user_management(request: Any) -> JsonResponse:
    """Handle user management operations."""
    users = User.objects.all()
    user_data = [
        {
            "id": user.id,
            "username": user.username,
            "email": user.email,
            "is_staff": user.is_staff,
            "is_active": user.is_active,
            "date_joined": user.date_joined.isoformat(),
        }
        for user in users
    ]

    return JsonResponse(
        {
            "users": user_data,
            "total_count": len(user_data),
            "message": "Users retrieved successfully",
        }
    )


def dashboard(request: Any) -> HttpResponse:
    """Render the admin dashboard."""
    context = {
        "total_users": User.objects.count(),
        "active_users": User.objects.filter(is_active=True).count(),
        "staff_users": User.objects.filter(is_staff=True).count(),
        "title": "pyOxide Admin Dashboard",
    }

    # For now, return a simple HTML response
    # Later we can create a proper template
    html_content = f"""
    <!DOCTYPE html>
    <html>
    <head>
        <title>{context['title']}</title>
        <style>
            body {{ font-family: Arial, sans-serif; margin: 40px; }}
            .dashboard {{ background: #f5f5f5; padding: 20px; border-radius: 10px; }}
            .stat {{ display: inline-block; margin: 10px; padding: 15px; 
                    background: white; border-radius: 5px; min-width: 150px; }}
            .stat h3 {{ margin: 0; color: #333; }}
            .stat p {{ margin: 5px 0 0 0; font-size: 24px; font-weight: bold; color: #007cba; }}
        </style>
    </head>
    <body>
        <h1>{context['title']}</h1>
        <div class="dashboard">
            <div class="stat">
                <h3>Total Users</h3>
                <p>{context['total_users']}</p>
            </div>
            <div class="stat">
                <h3>Active Users</h3>
                <p>{context['active_users']}</p>
            </div>
            <div class="stat">
                <h3>Staff Users</h3>
                <p>{context['staff_users']}</p>
            </div>
        </div>
        <p><a href="/admin/">Go to Django Admin Interface</a></p>
        <p><a href="/">Back to pyOxide Home</a></p>
    </body>
    </html>
    """

    return HttpResponse(html_content)
