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

"""
Shard management for Motor City Online server infrastructure.
"""

from dataclasses import dataclass
from typing import Dict, List, Optional


@dataclass
class Shard:
    """Represents a Motor City Online game shard."""

    name: str
    description: str
    id: int
    login_server_ip: str
    login_server_port: int
    lobby_server_ip: str
    lobby_server_port: int
    mcots_server_ip: str
    status_id: int
    status_reason: str
    server_group_name: str
    population: int
    max_personas_per_user: int
    diagnostic_server_host: str
    diagnostic_server_port: int

    def to_ini_format(self) -> str:
        """Convert shard to Motor City Online INI format."""
        return f"""[{self.name}]
Description={self.description}
ShardId={self.id}
LoginServerIP={self.login_server_ip}
LoginServerPort={self.login_server_port}
LobbyServerIP={self.lobby_server_ip}
LobbyServerPort={self.lobby_server_port}
MCOTSServerIP={self.mcots_server_ip}
StatusId={self.status_id}
Status_Reason={self.status_reason}
ServerGroup_Name={self.server_group_name}
Population={self.population}
MaxPersonasPerUser={self.max_personas_per_user}
DiagnosticServerHost={self.diagnostic_server_host}
DiagnosticServerPort={self.diagnostic_server_port}"""


class ShardManager:
    """Manages Motor City Online game shards."""

    def __init__(self) -> None:
        """Initialize the shard manager with default shards."""
        self._shards: Dict[int, Shard] = {}
        self._initialize_default_shards()

    def _initialize_default_shards(self) -> None:
        """Initialize default shards for Motor City Online."""
        default_shards = [
            Shard(
                name="Motor City Main",
                description="Primary Motor City Online server",
                id=1,
                login_server_ip="rusty-motors.com",
                login_server_port=8226,
                lobby_server_ip="rusty-motors.com",
                lobby_server_port=7003,
                mcots_server_ip="rusty-motors.com",
                status_id=0,  # 0 = no issues
                status_reason="",
                server_group_name="Group-1",
                population=150,
                max_personas_per_user=5,
                diagnostic_server_host="rusty-motors.com",
                diagnostic_server_port=80,
            ),
        ]

        for shard in default_shards:
            self._shards[shard.id] = shard

    def add_shard(self, shard: Shard) -> None:
        """Add a new shard to the manager."""
        self._shards[shard.id] = shard

    def remove_shard(self, shard_id: int) -> bool:
        """Remove a shard by ID. Returns True if removed, False if not found."""
        if shard_id in self._shards:
            del self._shards[shard_id]
            return True
        return False

    def get_shard(self, shard_id: int) -> Optional[Shard]:
        """Get a shard by ID."""
        return self._shards.get(shard_id)

    def get_all_shards(self) -> List[Shard]:
        """Get all shards sorted by ID."""
        return sorted(self._shards.values(), key=lambda s: s.id)

    def update_shard_population(self, shard_id: int, population: int) -> bool:
        """Update shard population. Returns True if updated, False if not found."""
        if shard_id in self._shards:
            self._shards[shard_id].population = population
            return True
        return False

    def update_shard_status(
        self, shard_id: int, status_id: int, status_reason: str
    ) -> bool:
        """Update shard status. Returns True if updated, False if shard not found."""
        if shard_id in self._shards:
            self._shards[shard_id].status_id = status_id
            self._shards[shard_id].status_reason = status_reason
            return True
        return False

    def generate_shard_list(self) -> str:
        """Generate the complete shard list in Motor City Online INI format."""
        shards = self.get_all_shards()
        if not shards:
            return ""

        return "\n\n".join(shard.to_ini_format() for shard in shards)

    def get_shard_count(self) -> int:
        """Get the total number of shards."""
        return len(self._shards)

    def get_online_shards(self) -> List[Shard]:
        """Get all shards that are online (status_id == 1)."""
        return [shard for shard in self._shards.values() if shard.status_id == 1]

    def get_total_population(self) -> int:
        """Get the total population across all online shards."""
        return sum(shard.population for shard in self.get_online_shards())
