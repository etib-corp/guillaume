from typing import Optional
from uuid import uuid1

from .properties import Properties


class Component:
    def __init__(self, properties: Optional[Properties] = None):
        self.properties = properties
        self.id = uuid1()
        self.parent: Optional["Component"] = None
        self.children: list["Component"] = []

    def set_parent(self, parent: Optional["Component"]) -> None:
        self.parent = parent
        if parent is not None and self not in parent.children:
            parent.add_child(self)

    def add_child(self, child: "Component") -> None:
        if child not in self.children:
            self.children.append(child)
        if child.parent is not self:
            child.set_parent(self)

    def remove_child(self, child: "Component") -> None:
        if child in self.children:
            self.children.remove(child)
            child.set_parent(None)

    def render(self) -> None:
        raise NotImplementedError("Subclasses must implement the render method")
