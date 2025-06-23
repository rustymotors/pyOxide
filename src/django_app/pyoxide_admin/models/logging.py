# Copyright (C) 2025 rustymotors
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

"""Logging and monitoring models for pyOxide."""

from typing import Any

from django.db import models


class ServerStatus(models.Model):
    """Log entries for server operations."""

    timestamp: Any = models.DateTimeField(auto_now_add=True)
    level: Any = models.CharField(
        max_length=20,
        choices=[
            ("INFO", "Info"),
            ("WARNING", "Warning"),
            ("ERROR", "Error"),
            ("DEBUG", "Debug"),
        ],
    )
    message: Any = models.TextField()
    source: Any = models.CharField(max_length=50, default="pyOxide")

    class Meta:
        ordering = ["-timestamp"]

    def __str__(self) -> str:
        return f"{self.timestamp} - {self.level}: {self.message[:50]}"


class APIRequest(models.Model):
    """Track API requests for monitoring."""

    timestamp: Any = models.DateTimeField(auto_now_add=True)
    method: Any = models.CharField(max_length=10)
    path: Any = models.CharField(max_length=200)
    status_code: Any = models.IntegerField()
    response_time_ms: Any = models.FloatField(null=True, blank=True)
    user_agent: Any = models.TextField(blank=True)
    ip_address: Any = models.GenericIPAddressField(null=True, blank=True)

    class Meta:
        ordering = ["-timestamp"]

    def __str__(self) -> str:
        return f"{self.method} {self.path} - {self.status_code}"
