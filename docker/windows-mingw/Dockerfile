FROM stateoftheartio/qt6:6.7-mingw-aqt@sha256:8320700cd6d43bab14cfce70ca84c8376ae182ab8c55a67ba847c717b58398bb

LABEL org.opencontainers.image.title="gLabels Qt Windows MinGW Builder"
LABEL org.opencontainers.image.description="Reproducible Qt 6.7 Windows build environment for gLabels Qt"
LABEL org.opencontainers.image.source="https://github.com/Tzu-Chi-Malaysia-IT/glabels-qt-sticker-printing"

USER root

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
        ca-certificates \
        git \
        zip \
    && rm -rf /var/lib/apt/lists/*

USER user
WORKDIR /workspace
