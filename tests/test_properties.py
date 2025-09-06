import pytest

from guillaume import Properties


class TestProperties:
    def test_properties_creation(self) -> None:
        props = Properties()
        assert isinstance(props, Properties)

    def test_set_and_get_property(self) -> None:
        props = Properties()
        props.set("key1", "value1")
        assert props.get("key1") == "value1"

    def test_get_nonexistent_property(self) -> None:
        props = Properties()
        assert props.get("nonexistent") is None

    def test_overwrite_property(self) -> None:
        props = Properties()
        props.set("key1", "value1")
        props.set("key1", "value2")
        assert props.get("key1") == "value2"
