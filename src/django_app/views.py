"""Django views for pyOxide admin interface."""

import json
from typing import Any

from django.contrib.auth.models import User
from django.http import HttpResponse, JsonResponse
from django.views.decorators.csrf import csrf_exempt
from django.views.decorators.http import require_http_methods


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

    # Default return for any other method (should not reach here due to decorator)
    return JsonResponse({"error": "Method not allowed"}, status=405)


def user_management(request: Any) -> JsonResponse:
    """Handle user management operations."""
    users = User.objects.all()
    user_data = [
        {
            "id": getattr(user, "id", None),  # type: ignore[attr-defined]
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
    """Render the admin dashboard using Jinja2 template."""
    import os

    from jinja2 import Environment, FileSystemLoader

    # Set up Jinja2 environment
    template_dir = os.path.join(
        os.path.dirname(os.path.dirname(os.path.dirname(__file__))), "templates"
    )
    env = Environment(loader=FileSystemLoader(template_dir))
    template = env.get_template("django_dashboard.html")

    context = {
        "total_users": User.objects.count(),
        "active_users": User.objects.filter(is_active=True).count(),
        "staff_users": User.objects.filter(is_staff=True).count(),
        "title": "pyOxide Admin Dashboard",
    }

    html_content = template.render(context)
    return HttpResponse(html_content)
