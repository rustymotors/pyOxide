"""Admin configuration for pyOxide models."""

from typing import Any

from django.contrib import admin

from .models import APIRequest, AuthSessions, AuthUsers, Configuration, ServerStatus


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

    def get_readonly_fields(self, request: Any, obj: Any = None) -> Any:
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


@admin.register(AuthSessions)
class AuthSessionsAdmin(admin.ModelAdmin):
    """Admin interface for authentication sessions."""

    list_display = (
        "ticket_short",
        "customer_username",
        "created_at",
        "expires_at",
        "is_active",
        "is_expired_display",
    )
    list_filter = ("is_active", "created_at", "expires_at")
    search_fields = ("ticket", "customer_id__username")
    readonly_fields = ("ticket", "created_at", "updated_at")
    ordering = ("-created_at",)

    def ticket_short(self, obj: Any) -> str:
        """Display shortened ticket for readability."""
        return f"{obj.ticket[:8]}...{obj.ticket[-4:]}"

    ticket_short.short_description = "Ticket"  # type: ignore[attr-defined]

    def customer_username(self, obj: Any) -> str:
        """Display username from related AuthUsers."""
        return getattr(obj.customer_id, "username", "Unknown")

    customer_username.short_description = "Username"  # type: ignore[attr-defined]

    def is_expired_display(self, obj: Any) -> bool:
        """Display if session is expired."""
        return obj.is_expired()

    is_expired_display.short_description = "Expired"  # type: ignore[attr-defined]
    is_expired_display.boolean = True  # type: ignore[attr-defined]
