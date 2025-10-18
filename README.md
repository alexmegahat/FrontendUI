<a name="readme-top"></a>

<div align="center">
  <h1 align="center">FrontendUI</h1>
</div>

<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
    </li>
    <li><a href="#license">License</a></li>
  </ol>
</details>

## About The Project

<strong>Frontend UI</strong> is a project containing non-plugin version of <strong>AdvancedGameUserInterface</strong>. It is a fully-featured, AAA-quality frontend UI system 
built using Unreal Engine 5’s Common UI framework and C++.

System contains:
- A modular UI framework using Unreal Engine 5's Common UI system and C++;
- Full gamepad support;
- Widgets managed using widget stacks with gameplay tags and async loading;
- Fully functional options menu (audio & video settings, a complete input remapping system and more);
- Tab navigation, Dynamic details panel, Modular setting categories;
- Game User Settings system for saving/loading config values;
- Input preprocessors to detect and process player input dynamically;
- Start Up and In-Game Loading Screen;
- System uses Common Activatable Widget, Widget Stacks, Common Text Block and more;
- Custom Data Objects for List View Item generation.

<strong>Plugin</strong> version Setup:

1. There should be 2 new tabs in the Project Settings under the Game section:

<img width="550" height="444" alt="image" src="https://github.com/user-attachments/assets/7b88b246-a0d0-488f-a72d-13a044eb9d83" />

And also there should be more gameplay tags assigned in the project’s Gameplay Tag Manager:

<img width="759" height="620" alt="image" src="https://github.com/user-attachments/assets/803ca50e-7711-46ee-a400-21a9fcba174f" />

2. If this is done correctly, then set up Project Settings as displayed:

<img width="2529" height="802" alt="image" src="https://github.com/user-attachments/assets/2c96147a-0ff5-4395-ba75-cd5ea1d97b1e" />

<img width="2529" height="447" alt="image" src="https://github.com/user-attachments/assets/76a99455-0b93-4083-8f79-83f79b33475c" />

<img width="1786" height="1052" alt="image" src="https://github.com/user-attachments/assets/29d30ab3-6fd7-48fa-b4dc-ee9fe3edc8de" />

<img width="2125" height="776" alt="image" src="https://github.com/user-attachments/assets/1bbfe5aa-83d1-4157-855d-6005abfc70f2" />

<img width="2124" height="1053" alt="image" src="https://github.com/user-attachments/assets/2637f741-eb5f-42c2-a090-14e514ab78f3" />

<img width="2127" height="276" alt="image" src="https://github.com/user-attachments/assets/1e8a28b7-f341-4b87-8161-f71425c78624" />

3. Add startup movies (studio logo, ue logo video sequence), it will display automatically on game startup (before the main menu) and Loading Screen should pick up on that and not display Loading Screen if the movie is being displayed. Movie files should be put in Game/Content/Movies/

<img width="2676" height="426" alt="image" src="https://github.com/user-attachments/assets/c08af9d6-7eca-4a25-9cda-694ecbf2a5cd" />

<p style="text-align: end">(<a href="#readme-top">back to top</a>)</p>

<!-- LICENSE -->
## License

Copyright © 2025, [Aleksei Miakishev](https://github.com/alexmegahat/). All rights reserved.

Assets rights are reserved to [Vince Petrelli](https://github.com/vinceright3)

<p style="text-align: end">(<a href="#readme-top">back to top</a>)</p>

<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
