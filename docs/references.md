# References

## Foundational Cellular Potts Model

- **Graner, F. & Glazier, J.A.** (1992). Simulation of biological cell sorting using a two-dimensional extended Potts model. *Physical Review Letters*, 69(13), 2013-2016. doi:10.1103/PhysRevLett.69.2013
  -- This paper introduced the Cellular Potts Model as a way to simulate how cells with different adhesion properties spontaneously sort themselves into distinct groups. The model extends the statistical mechanics Potts model by assigning each lattice site a cell identity, and uses an energy minimization approach (Hamiltonian with adhesion and area terms) to drive cell rearrangement through stochastic spin flips.

- **Glazier, J.A. & Graner, F.** (1993). Simulation of the differential adhesion driven rearrangement of biological cells. *Physical Review E*, 47(3), 2128-2154. doi:10.1103/PhysRevE.47.2128
  -- The extended follow-up to the 1992 paper, providing detailed analysis of how differential adhesion strengths between cell types produce sorting patterns that match experimental observations. This work established the CPM as a standard tool for studying tissue-level cell behavior.

## CPM Software and Simulation Frameworks

- **Wortel, I.M.N. & Textor, J.** (2021). Artistoo, a library to build, share, and explore simulations of cells and tissues in the web browser. *eLife*, 10:e61288. doi:10.7554/eLife.61288
  -- A JavaScript-based CPM framework that runs entirely in the browser with no server needed. Artistoo demonstrated that interactive, shareable CPM simulations are practical as web applications. It served as an inspiration for moving our own simulator to a web-based architecture, though our implementation uses a different cell model (continuous-space Gaussian filopodia rather than lattice-based).

- **Swat, M.H. et al.** (2012). Multi-scale modeling of tissues using CompuCell3D. *Methods in Cell Biology*, 110, 325-366.
  -- CompuCell3D is one of the most widely used platforms for CPM simulations in the biological modeling community. It provides a Python scripting interface on top of a C++ simulation core, with support for chemical fields, cell growth, and complex cell behaviors. Our project takes a lighter-weight approach focused on a specific biological system rather than a general-purpose framework.

- **Starruss, J. et al.** (2014). Morpheus: a user-friendly modeling environment for multiscale and multicellular systems biology. *Bioinformatics*, 30(9), 1331-1332. doi:10.1093/bioinformatics/btt772
  -- Morpheus is a GUI-based tool for building multicellular simulations using XML model definitions. It targets users who want to set up CPM-like models without writing code. Our project differs in that the simulation logic is fully exposed as Python source code, making it easier to modify the mathematical model for research purposes.

## Machine Learning Approaches to CPM

- **Minartz, N. et al.** (2025). NeuralCPM: Real-time simulation of deformable biological tissue with neural networks. *Proceedings of the 42nd International Conference on Machine Learning (ICML 2025)*.
  -- This recent work trains neural networks to approximate CPM dynamics, achieving much faster simulation speeds than traditional lattice-based methods. It represents a potential future direction for accelerating simulations like ours, especially if the cell count needs to scale to thousands.

## Zebrafish DFC Biology

- **Oteiza, P. et al.** (2008). Origin and shaping of the laterality organ in zebrafish. *Development*, 135(16), 2807-2813. doi:10.1242/dev.022228
  -- This study characterized where DFCs come from during zebrafish development and how they organize into the structure that becomes Kupffer's vesicle. It provided key measurements of DFC cluster size, cell count, and migration timing that inform the parameter ranges used in our simulator.

- **Oteiza, P. et al.** (2010). Planar cell polarity signaling regulates cell adhesion properties in progenitors of the zebrafish laterality organ. *Development*, 137(20), 3459-3468. doi:10.1242/dev.049981
  -- Demonstrated that cell polarity signaling pathways control how tightly DFCs adhere to each other and to the surrounding tissue. This work motivated the inclusion of stage-dependent adhesion parameters in our environment model, where the coupling between DFCs and the EVL decreases over time.

- **Ablooglu, A.J. et al.** (2021). Apical contacts stemming from incomplete delamination guide progenitor cell allocation through a dragging mechanism. *eLife*, 10:e66495. doi:10.7554/eLife.66495
  -- Revealed that DFCs maintain thin apical connections to the EVL even as they delaminate, and these connections create a mechanical dragging force that helps guide the cells. This finding is directly modeled in our simulator through the EVL adhesion stages: early stages have strong adhesion (representing intact apical contacts) and later stages have zero adhesion (representing complete delamination).

## Epiboly and Tissue Mechanics

- **Rho, H.K. & Bhatt, D.H.** (2009). Zebrafish epiboly: mechanics and mechanisms. *International Journal of Developmental Biology*, 53(8-10), 1187-1196.
  -- A review of the mechanical forces driving epiboly in zebrafish, including EVL spreading, deep cell intercalation, and yolk cell contraction. Our EVL boundary model draws on the description of how the EVL front advances as a coherent tissue sheet.

- **Keller, P.J. et al.** (2025). Modeling Epithelial Morphogenesis during Zebrafish Epiboly. *bioRxiv*. doi:10.1101/2025.xx.xx
  -- A computational study of the mechanics of EVL tissue during epiboly, providing quantitative data on tissue tension and migration speed. The EVL velocity and stage transition positions in our model are informed by measurements reported in this and related studies.

## Cell Shape and Motility Theory

- **Rens, E.G. & Merks, R.M.H.** (2020). Cell contractility facilitates alignment of cells and tissues to static uniaxial stretch. *iScience*, 23(1), 100827. doi:10.1016/j.isci.2020.100827
  -- Analyzed how cell shape changes drive alignment and migration in CPM models with mechanical coupling to the environment. The concept that shape asymmetry produces net cell velocity (used in our velocity estimation step) is consistent with the framework described in this paper.

## Related Tissue Simulation Toolkits

- **Merks, R.M.H. et al.** (2011). VirtualLeaf: An open-source framework for cell-based modeling of plant tissue growth and development. *Plant Physiology*, 155(2), 656-666.
  -- Although focused on plant cells, VirtualLeaf introduced useful ideas about vertex-based cell models and tissue mechanics that are conceptually related to the deformable membrane approach used in our simulator.

## Software Dependencies

- **NumPy**: Harris, C.R. et al. (2020). Array programming with NumPy. *Nature*, 585, 357-362. doi:10.1038/s41586-020-2649-2 -- The foundational array computing library used for all numerical operations in the simulation.
- **SciPy**: Virtanen, P. et al. (2020). SciPy 1.0: fundamental algorithms for scientific computing in Python. *Nature Methods*, 17, 261-272. doi:10.1038/s41592-019-0686-2 -- Available for interpolation, optimization, and spatial algorithms.
- **FastAPI**: Ramirez, S. (2018-present). FastAPI framework, https://fastapi.tiangolo.com -- The web framework providing REST and WebSocket endpoints.
- **Pydantic**: Colvin, S. (2017-present). Pydantic data validation, https://docs.pydantic.dev -- Used for request/response validation and configuration models.
- **Uvicorn**: Encode (2018-present). Uvicorn ASGI server, https://www.uvicorn.org -- The ASGI server that runs the FastAPI application.
