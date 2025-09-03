from typing import Dict, Optional


class Properties:
    def __init__(self):
        self.dictionary: Dict[str, Optional[str | int | float | bool]] = {}

    def set(self, key: str, value: Optional[str | int | float | bool]) -> None:
        self.dictionary[key] = value

    def get(self, key: str) -> Optional[str | int | float | bool]:
        return self.dictionary.get(key, None)

    def remove(self, key: str) -> None:
        self.dictionary.pop(key, None)

    def clear(self) -> None:
        self.dictionary.clear()
    

