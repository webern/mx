FROM matthewjamesbriggs/mxci:v001

ENV BASEDIR="/mx"
WORKDIR $BASEDIR
COPY . .
RUN chmod +x .circleci/dockerfile-test-entrypoint.sh
CMD [".circleci/dockerfile-test-entrypoint.sh"]
