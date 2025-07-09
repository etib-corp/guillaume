# Guillaume Project: Detailed Development Roadmap

## Description

This document provides a detailed development roadmap for the Guillaume project, a C++ library for building graphical interfaces and multimedia applications. The roadmap is based on source code structure and tracked git commits, outlining precise steps and deliverables for each phase.

## Development Roadmap

| Phase | Tasks | Deliverables | Status |
|-------|-------|--------------|--------|
| **1. Project Initialization** | - Create repository- Define objectives and features- Set up CI/CD | - GitHub repo- Initial README- CI/CD config | Completed |
| **2. Core Architecture** | - Implement window management- Set up rendering pipeline- Establish event loop | - `Conctext`- `Renderer`- Event loop code | Completed |
| **3. Graphic Primitives** | - Add shapes (rectangles, circles, lines)- Implement color and style APIs | - `Primitive`- Unit tests for primitives | In progress |
| **4. Input & Event System** | - Keyboard/mouse event handling- Event dispatching- Cross-platform input abstraction | - `Event`- Platform adapters- Input tests | In progress |
| **5. Resource Management** | - Texture and font loading- Asset caching- Memory management | - `Renderer`- Asset loaders | Upcoming |
| **6. Platform Support** | - Windows, Linux, macOS support- Platform abstraction layer- Build scripts for each OS | - Platform-specific code- Build/test reports | Upcoming |
| **7. Testing & Validation** | - Automated unit/integration tests- Cross-platform CI builds- Bug tracking | - Test suite- CI results- Bug reports | Upcoming |
| **8. Documentation** | - API reference- Developer guides- Example projects | - Markdown docs- Tutorials- Example code | Upcoming |
| **9. Release & Distribution** | - Version tagging- Release notes- Binary packaging | - GitHub releases- Changelog- Install scripts | Upcoming |
| **10. Maintenance & Community** | - Issue triage- Pull request reviews- Community support | - Issue tracker- Contribution guidelines | Ongoing |
| **11. Extensions & Improvements** | - New widgets/components- Performance optimizations- Third-party integrations | - Feature branches- Release notes | Ongoing |

## Additional Notes

- Each phase is tracked via git commits and issues for transparency.
- Contributions are welcome; see [CONTRIBUTING.md](https://github.com/etib-corp/guillaume/blob/main/CONTRIBUTING.md).
- For the latest updates, visit the [GitHub repository](https://github.com/etib-corp/guillaume).
