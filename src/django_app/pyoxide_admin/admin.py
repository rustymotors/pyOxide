"""Admin configuration for pyOxide models."""

from typing import Any

from django.contrib import admin

from .models import APIRequest, AuthUsers, Configuration, ServerStatus


@admin.register(ServerStatus)
class ServerStatusAdmin(admin.ModelAdmin):
    """Admin interface for server logs."""

    list_display = ("timestamp", "level", "source", "message_short")
    list_filter = ("level", "source", "timestamp")
    search_fields = ("message", "source")
    readonly_fields = ("timestamp",)

    def message_short(self, obj: Any) -> str:
        """Return shortened message for list display."""
        return obj.message[:50] + "..." if len(obj.message) > 50 else obj.message

    message_short.short_description = "Message"  # type: ignore[attr-defined]


@admin.register(APIRequest)
class APIRequestAdmin(admin.ModelAdmin):
    """Admin interface for API request tracking."""

    list_display = (
        "timestamp",
        "method",
        "path",
        "status_code",
        "response_time_ms",
        "ip_address",
    )
    list_filter = ("method", "status_code", "timestamp")
    search_fields = ("path", "ip_address", "user_agent")
    readonly_fields = ("timestamp",)


@admin.register(Configuration)
class ConfigurationAdmin(admin.ModelAdmin):
    """Admin interface for configuration settings."""

    list_display = ("key", "value_short", "updated_at")
    search_fields = ("key", "description")
    readonly_fields = ("created_at", "updated_at")

    def value_short(self, obj: Any) -> str:
        """Return shortened value for list display."""
        return obj.value[:30] + "..." if len(obj.value) > 30 else obj.value

    value_short.short_description = "Value"  # type: ignore[attr-defined]


@admin.register(AuthUsers)
class AuthUsersAdmin(admin.ModelAdmin):  # type: ignore[type-arg]
    """Admin interface for authentication users."""

    list_display = (
        "customer_id",
        "username",
        "email",
        "is_active",
        "created_at",
        "last_login",
    )
    list_filter = ("is_active", "created_at")
    search_fields = ("username", "email")
    readonly_fields = (
        "customer_id",
        "password_hash",
        "created_at",
        "updated_at",
        "last_login",
    )

    fields = (
        "customer_id",
        "username",
        "email",
        "is_active",
        "password_hash",
        "created_at",
        "updated_at",
        "last_login",
    )

    def get_readonly_fields(self, request: Any, obj: Any = None) -> tuple[str, ...]:
        """Make password_hash always readonly - use set_password method instead."""
        if obj:  # Editing existing user
            return self.readonly_fields
        else:  # Creating new user
            return ("customer_id", "created_at", "updated_at", "last_login")

    def save_model(self, request: Any, obj: Any, form: Any, change: bool) -> None:
        """Custom save to handle password setting."""
        if not change:  # New user
            # For new users, set a default password (form should handle this)
            obj.set_password("temp_password_123")
        super().save_model(request, obj, form, change)
