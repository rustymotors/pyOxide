# Generated by Django 5.2.3 on 2025-06-22 12:21

from django.db import migrations, models


class Migration(migrations.Migration):

    initial = True

    dependencies = []

    operations = [
        migrations.CreateModel(
            name="APIRequest",
            fields=[
                (
                    "id",
                    models.BigAutoField(
                        auto_created=True,
                        primary_key=True,
                        serialize=False,
                        verbose_name="ID",
                    ),
                ),
                ("timestamp", models.DateTimeField(auto_now_add=True)),
                ("method", models.CharField(max_length=10)),
                ("path", models.CharField(max_length=200)),
                ("status_code", models.IntegerField()),
                ("response_time_ms", models.FloatField(blank=True, null=True)),
                ("user_agent", models.TextField(blank=True)),
                ("ip_address", models.GenericIPAddressField(blank=True, null=True)),
            ],
            options={
                "ordering": ["-timestamp"],
            },
        ),
        migrations.CreateModel(
            name="Configuration",
            fields=[
                (
                    "id",
                    models.BigAutoField(
                        auto_created=True,
                        primary_key=True,
                        serialize=False,
                        verbose_name="ID",
                    ),
                ),
                ("key", models.CharField(max_length=100, unique=True)),
                ("value", models.TextField()),
                ("description", models.TextField(blank=True)),
                ("created_at", models.DateTimeField(auto_now_add=True)),
                ("updated_at", models.DateTimeField(auto_now=True)),
            ],
        ),
        migrations.CreateModel(
            name="ServerLog",
            fields=[
                (
                    "id",
                    models.BigAutoField(
                        auto_created=True,
                        primary_key=True,
                        serialize=False,
                        verbose_name="ID",
                    ),
                ),
                ("timestamp", models.DateTimeField(auto_now_add=True)),
                (
                    "level",
                    models.CharField(
                        choices=[
                            ("INFO", "Info"),
                            ("WARNING", "Warning"),
                            ("ERROR", "Error"),
                            ("DEBUG", "Debug"),
                        ],
                        max_length=20,
                    ),
                ),
                ("message", models.TextField()),
                ("source", models.CharField(default="pyOxide", max_length=50)),
            ],
            options={
                "ordering": ["-timestamp"],
            },
        ),
    ]
