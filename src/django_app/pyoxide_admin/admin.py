"""Admin configuration for pyOxide models."""

from django.contrib import admin

from .models import APIRequest, Configuration, ServerLog


@admin.register(ServerLog)
class ServerLogAdmin(admin.ModelAdmin):
    """Admin interface for server logs."""

    list_display = ("timestamp", "level", "source", "message_short")
    list_filter = ("level", "source", "timestamp")
    search_fields = ("message", "source")
    readonly_fields = ("timestamp",)

    def message_short(self, obj: ServerLog) -> str:
        """Return shortened message for list display."""
        return obj.message[:50] + "..." if len(obj.message) > 50 else obj.message

    message_short.short_description = "Message"


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

    def value_short(self, obj: Configuration) -> str:
        """Return shortened value for list display."""
        return obj.value[:30] + "..." if len(obj.value) > 30 else obj.value

    value_short.short_description = "Value"
