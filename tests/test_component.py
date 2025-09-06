import pytest

from guillaume import Component
from guillaume import Properties


class TestComponent:
    def test_component_creation(self) -> None:
        comp = Component()
        assert isinstance(comp, Component)

    def test_component_with_properties(self) -> None:
        props = Properties()
        comp = Component(properties=props)
        assert comp.properties is props

    def test_add_child(self) -> None:
        parent = Component()
        child = Component()
        parent.add_child(child)
        assert child in parent.children

    def test_remove_child(self) -> None:
        parent = Component()
        child = Component()
        parent.add_child(child)
        parent.remove_child(child)
        assert child not in parent.children

    def test_render_not_implemented(self) -> None:
        comp = Component()
        with pytest.raises(NotImplementedError):
            comp.render()
