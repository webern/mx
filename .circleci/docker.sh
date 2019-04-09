THIS_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"
MX_DIR="$THIS_DIR/.."
cd "$THIS_DIR"

docker build \
    -f "$THIS_DIR/Dockerfile" \
    -t matthewjamesbriggs/mxci:v001 \
    "$THIS_DIR"

docker rm -f mxtestcontainer || true;
docker rmi -f mxtestimage || true;

docker build \
    -f "$THIS_DIR/Dockerfile.test" \
    -t mxtestimage \
    "$MX_DIR"

docker run -it --name mxtestcontainer mxtestimage