from typing import Optional

from .component import Component
from .properties import Properties


class Application(Component):
    def __init__(self, properties: Optional[Properties] = None):
        super().__init__(properties)

    def render(self) -> None:
        for child in self.children:
            child.render()
