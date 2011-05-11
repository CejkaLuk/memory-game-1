cd $(dirname $0)/..
PROJECT=$(basename `pwd`)
VERSION=`git tag | sed -e "s/v//g"`
git archive --format=tar --prefix=$PROJECT-$VERSION/ v$VERSION | gzip -n > $PROJECT-$VERSION.tar.gz

mv $PROJECT-$VERSION.tar.gz ~/rpmbuild/SOURCES/
cp rpm/$PROJECT.spec ~/rpmbuild/SPECS/
rpmbuild -ba ~/rpmbuild/SPECS/$PROJECT.spec
