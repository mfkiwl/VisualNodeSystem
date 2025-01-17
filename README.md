# VisualNode System

![build](https://github.com/Azzinoth/VisualNodeSystem/actions/workflows/Build.yml/badge.svg?branch=master)

_**In the near future, I plan to make this repo more lightweight and much easier to integrate into your projects. It will no longer include the submodule 'FEBasicApplication'. This submodule has been used to consolidate useful functionalities for all my graphics-related projects in one place. 'FEBasicApplication' includes a partial but unmodified version of the Dear ImGui docking branch.**_

This library provides a powerful framework for creating and managing visual node systems. These are typically used in graphical programming environments, game logic builders, and AI behavior tree editors. I have personally used it in two of my projects:

[Focal Engine Editor](https://github.com/Azzinoth/FocalEngineEditor)
![Focal Engine editor material window](https://github.com/Azzinoth/VisualNodeSystem/blob/media/Focal%20Engine%20Editor%20example.png)

[Focal Engine Test Platform](https://github.com/Azzinoth/FocalEngineTestPlatform)
![Focal Engine test platform](https://github.com/Azzinoth/VisualNodeSystem/blob/media/Test%20Platform%20example.png)

## Key Features

- (_Soon_) **Unchanged Dear Imgui**: This project does not depend on modifications to Dear Imgui.

- **Smooth Zoom**: A precise and dynamic zoom functionality, allowing for detailed viewing and efficient navigation.

- **Reroute Nodes**: Reroute nodes offer enhanced organization for clearer, more readable visual graphs, enabling flexible customization of connection paths. Also they simplify debugging by making connections traceable. To add reroute node just double click on connection.
<div align="center">
    <img src="https://github.com/Azzinoth/VisualNodeSystem/blob/media/Reroute%20Node%20example.png" width="60%">
</div>

- **Flexible Socket Management**: The library offers functionalities for managing node sockets (input/output). These sockets serve as points of data connection between different nodes.

- **Visual Node Area Management**: This feature allows you to manage a visual node area, offering several operations like getting selected nodes, connecting nodes, rendering, and event propagation.

- **JSON Serialization**: The library provides functionalities to serialize/deserialize the node data to/from JSON format.

- **Node Factory For Child Node Creation**: The library includes a system for creating child nodes using node factory, which enables JSON serialization of custom nodes.

- **Custom Context Menus**: The library supports the integration of custom context menus.

- **File System Integration**: The system allows you to save/load node data to/from files.

- **Node Event Callbacks**: The library provides functionalities to set callbacks for node events.

## Usage

For a simple example of how to use library, see the [VisualNodeSystem Example](https://github.com/Azzinoth/VisualNodeSystem-Example).

To add this module to your project, use the following command:

```bash
git submodule add https://github.com/Azzinoth/VisualNodeSystem
```

If you want to move the submodule to a folder named "SubSystems", for example, use the following command:

```bash
git mv VisualNodeSystem SubSystems/
```

## Third Party Licenses

This project uses the following third-party libraries:

1) **GLM**: This library is licensed under a permissive open-source license, similar to the MIT license. The full license text can be found at [GLM's GitHub repository](https://github.com/g-truc/glm/blob/master/copying.txt).

2) **jsoncpp**: This library is licensed under the MIT License. The full license text can be found at [jsoncpp's GitHub repository](https://github.com/open-source-parsers/jsoncpp/blob/master/LICENSE).