from guillaume import Application
from guillaume import Component


class TestApplication:
    def test_application_creation(self):
        app = Application()
        assert isinstance(app, Application)

    def test_application_with_properties(self):
        from guillaume import Properties

        props = Properties()
        app = Application(properties=props)
        assert app.properties is props

    def test_render_calls_children(self):
        class MockChild(Component):
            def __init__(self):
                super().__init__()
                self.render_called = False

            def render(self):
                self.render_called = True

        app = Application()
        child1 = MockChild()
        child2 = MockChild()
        app.add_child(child1)
        app.add_child(child2)

        app.render()

        assert child1.render_called
        assert child2.render_called
