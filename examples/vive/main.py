#!/usr/bin/python3

## import avango-guacamole libraries
import avango
import avango.gua
import avango.script
import avango.vive


class ButtonReader(avango.script.Script):

    def __init__(self):
        self.super(ButtonReader).__init__()

    def assign_window(self, VIVE_WINDOW):
        self.window = VIVE_WINDOW
        self.latest_trigger_value = 0.0
        self.always_evaluate(True)

    def evaluate(self):
        if self.window.Controller0TriggerButtonPressed.value:
            print("Controller 0 Pressed: TRIGGER_BUTTON")

        if self.window.Controller0TriggerValue.value != self.latest_trigger_value:
            self.latest_trigger_value = self.window.Controller0TriggerValue.value
            print("Controller 0 Trigger Value Changed: " + str(self.latest_trigger_value))


def start():
    scenegraph = avango.gua.nodes.SceneGraph(Name = "scenegraph")
    
    # init scene
    loader = avango.gua.nodes.TriMeshLoader()
    monkey = loader.create_geometry_from_file("monkey", "data/objects/monkey.obj", avango.gua.LoaderFlags.NORMALIZE_SCALE | avango.gua.LoaderFlags.NORMALIZE_POSITION)
    monkey.Transform.value = avango.gua.make_trans_mat(0.0, 1.0, 0.0) * avango.gua.make_scale_mat(0.5)
    monkey.Material.value.set_uniform("Color", avango.gua.Vec4(1.0, 0.0, 0.0, 1))
    monkey.Material.value.set_uniform("Roughness", 0.2)
    monkey.Material.value.set_uniform("Metalness", 1.0)
    scenegraph.Root.value.Children.value.append(monkey)

    light = avango.gua.nodes.LightNode(Type = avango.gua.LightType.POINT,
                                       Brightness = 150.0)
    light.Transform.value = avango.gua.make_trans_mat(-3.0, 5.0, 5.0) * avango.gua.make_scale_mat(12)
    scenegraph.Root.value.Children.value.append(light)

    # vive window
    window = avango.vive.nodes.ViveWindow(Title = "vive_example")
    window.Size.value = window.Resolution.value
    window.EnableVsync.value = False
    window.EnableFullscreen.value = False
    avango.gua.register_window(window.Title.value, window)

    # viewing setup
    navigation_node = avango.gua.nodes.TransformNode(Name = "navigation")
    scenegraph.Root.value.Children.value.append(navigation_node)

    controller0 = loader.create_geometry_from_file("controller0", "data/objects/vive_controller/vive_controller.obj", avango.gua.LoaderFlags.LOAD_MATERIALS)
    controller1 = loader.create_geometry_from_file("controller1", "data/objects/vive_controller/vive_controller.obj", avango.gua.LoaderFlags.LOAD_MATERIALS)
    base0 = loader.create_geometry_from_file("base0", "data/objects/vive_lighthouse/vive_lighthouse.obj", avango.gua.LoaderFlags.LOAD_MATERIALS)
    base1 = loader.create_geometry_from_file("base1", "data/objects/vive_lighthouse/vive_lighthouse.obj", avango.gua.LoaderFlags.LOAD_MATERIALS)
    controller0.Transform.connect_from(window.Controller0SensorOrientation)
    controller1.Transform.connect_from(window.Controller1SensorOrientation)
    base0.Transform.connect_from(window.TrackingReference0SensorOrientation)
    base1.Transform.connect_from(window.TrackingReference1SensorOrientation)
    navigation_node.Children.value = [controller0, controller1, base0, base1]

    head_node = avango.gua.nodes.TransformNode(Name = "head")
    head_node.Transform.connect_from(window.HMDSensorOrientation)
    navigation_node.Children.value.append(head_node)

    left_screen_node = avango.gua.nodes.ScreenNode(Name = "left_screen_node",
                                                   Width = window.LeftScreenSize.value.x,
                                                   Height = window.LeftScreenSize.value.y,
                                                   Transform = avango.gua.make_trans_mat(window.LeftScreenTranslation.value))
    head_node.Children.value.append(left_screen_node)

    right_screen_node = avango.gua.nodes.ScreenNode(Name = "right_screen_node",
                                                    Width = window.RightScreenSize.value.x,
                                                    Height = window.RightScreenSize.value.y,
                                                    Transform = avango.gua.make_trans_mat(window.RightScreenTranslation.value))
    head_node.Children.value.append(right_screen_node)

    camera_node = avango.gua.nodes.CameraNode(Name = "camera_node",
                                              LeftScreenPath = left_screen_node.Path.value,
                                              RightScreenPath = right_screen_node.Path.value,
                                              SceneGraph = scenegraph.Name.value,
                                              Resolution = window.Resolution.value,
                                              OutputWindowName = window.Title.value,
                                              EyeDistance = window.EyeDistance.value,
                                              EnableStereo = True)
    head_node.Children.value.append(camera_node)

    # pipeline
    resolve_pass = avango.gua.nodes.ResolvePassDescription()
    resolve_pass.BackgroundMode.value = avango.gua.BackgroundMode.COLOR
    resolve_pass.BackgroundColor.value = avango.gua.Color(0.2, 0.2, 0.2)
    
    pipeline_description = avango.gua.nodes.PipelineDescription(Passes = [])
    pipeline_description.Passes.value.append(avango.gua.nodes.TriMeshPassDescription())
    pipeline_description.Passes.value.append(avango.gua.nodes.LightVisibilityPassDescription())
    pipeline_description.Passes.value.append(resolve_pass)
    pipeline_description.Passes.value.append(avango.gua.nodes.SSAAPassDescription())

    camera_node.PipelineDescription.value = pipeline_description

    # button reader
    button_reader = ButtonReader()
    button_reader.assign_window(window)

    # start application/render loop
    viewer = avango.gua.nodes.Viewer()
    viewer.SceneGraphs.value = [scenegraph]
    viewer.Windows.value = [window]
    viewer.DesiredFPS.value = 200.0
    viewer.run()


if __name__ == '__main__':
    start()
