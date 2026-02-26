# =============================================================================
# Multi-arch, Multi-stage Dockerfile: ROS2 Humble on Ubuntu 22.04
#
# STAGE 1 (voxl-dev)           — Full dev environment: compilers, debuggers, editors
#                            Used on the workstation for development & cross-build
#
# STAGE 2 (voxl-runtime) — Minimal runtime: only what's needed to RUN ROS2 nodes
#                            Used on the voxl drone for flight operations
# =============================================================================

FROM ubuntu:22.04 AS voxl-dev

ENV DEBIAN_FRONTEND=noninteractive

ENV LANG=C.UTF-8
ENV LC_ALL=C.UTF-8

# ---- Core system + build packages ----
RUN apt-get update && apt-get install -y --no-install-recommends \
    # Build tools
    build-essential \
    cmake \
    gdb \
    # Version control & networking
    git \
    wget \
    curl \
    ssh \
    # System utilities
    gnupg2 \
    lsb-release \
    software-properties-common \
    ca-certificates \
    sudo \
    # Dev comfort
    vim \
    # Python
    python3 \
    python3-pip \
    && rm -rf /var/lib/apt/lists/*

# ---- Install ROS2 Humble (full: base + build tools) ----
RUN curl -sSL https://raw.githubusercontent.com/ros/rosdistro/master/ros.key \
      -o /usr/share/keyrings/ros-archive-keyring.gpg \
    && echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/ros-archive-keyring.gpg] \
      http://packages.ros.org/ros2/ubuntu $(lsb_release -cs) main" \
      > /etc/apt/sources.list.d/ros2.list \
    && apt-get update && apt-get install -y --no-install-recommends \
      ros-humble-ros-base \
      ros-humble-rmw-cyclonedds-cpp \
      ros-humble-launch-testing \
      ros-humble-ament-cmake \
      python3-colcon-common-extensions \
      python3-rosdep \
      python3-vcstool \
    && rm -rf /var/lib/apt/lists/*

# ---- Initialize rosdep ----
RUN rosdep init || true && rosdep update --rosdistro humble

# ---- Default RMW and ROS setup ----
ENV RMW_IMPLEMENTATION=rmw_cyclonedds_cpp
RUN echo "source /opt/ros/humble/setup.bash" >> /etc/bash.bashrc

# ---- Create non-root user ----
ARG USERNAME=voxl-dev
ARG USER_UID=1000
ARG USER_GID=1000
RUN groupadd --gid ${USER_GID} ${USERNAME} \
    && useradd --uid ${USER_UID} --gid ${USER_GID} -m ${USERNAME} -s /bin/bash \
    && echo "${USERNAME} ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers.d/${USERNAME}

USER ${USERNAME}
WORKDIR /ros2_ws
RUN mkdir -p /ros2_ws/build /ros2_ws/install /ros2_ws/log

RUN echo "source /opt/ros/humble/setup.bash" >> /home/${USERNAME}/.bashrc \
    && echo 'if [ -f /ros2_ws/install/setup.bash ]; then source /ros2_ws/install/setup.bash; fi' \
       >> /home/${USERNAME}/.bashrc

CMD ["/bin/bash"]


# =============================================================================
# STAGE 2: VOXL RUNTIME (minimal — no compilers, no dev tools)
# =============================================================================
FROM ubuntu:22.04 AS voxl-runtime

ENV DEBIAN_FRONTEND=noninteractive

ENV LANG=C.UTF-8
ENV LC_ALL=C.UTF-8

# ---- Minimal runtime dependencies only ----
RUN apt-get update && apt-get install -y --no-install-recommends \
    curl \
    gnupg2 \
    lsb-release \
    ca-certificates \
    python3 \
    python3-pip \
    sudo \
    && rm -rf /var/lib/apt/lists/*

# ---- Install ROS2 Humble (runtime only — no ament-cmake, no launch-testing) ----
RUN curl -sSL https://raw.githubusercontent.com/ros/rosdistro/master/ros.key \
      -o /usr/share/keyrings/ros-archive-keyring.gpg \
    && echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/ros-archive-keyring.gpg] \
      http://packages.ros.org/ros2/ubuntu $(lsb_release -cs) main" \
      > /etc/apt/sources.list.d/ros2.list \
    && apt-get update && apt-get install -y --no-install-recommends \
      ros-humble-ros-base \
      ros-humble-rmw-cyclonedds-cpp \
    && rm -rf /var/lib/apt/lists/*

# ---- Default RMW and ROS setup ----
ENV RMW_IMPLEMENTATION=rmw_cyclonedds_cpp
RUN echo "source /opt/ros/humble/setup.bash" >> /etc/bash.bashrc

# ---- Create non-root user ----
ARG USERNAME=voxl
ARG USER_UID=1000
ARG USER_GID=1000
RUN groupadd --gid ${USER_GID} ${USERNAME} \
    && useradd --uid ${USER_UID} --gid ${USER_GID} -m ${USERNAME} -s /bin/bash \
    && echo "${USERNAME} ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers.d/${USERNAME}

USER ${USERNAME}
WORKDIR /ros2_ws
RUN mkdir -p /ros2_ws/build /ros2_ws/install /ros2_ws/log

RUN echo "source /opt/ros/humble/setup.bash" >> /home/${USERNAME}/.bashrc \
    && echo 'if [ -f /ros2_ws/install/setup.bash ]; then source /ros2_ws/install/setup.bash; fi' \
       >> /home/${USERNAME}/.bashrc

CMD ["/bin/bash"]
