Name:           memory-game
Version:        1.0
Release:        1%{?dist}
Summary:        Simple and good-looking memory game

Group:          Amusements/Games
License:        EUPL 1.1
URL:            https://gitorious.org/colorful-apps/memory-game

# The upstream release tarballs can be found here:
Source0:        http://venemo.fedorapeople.org/sources/%{name}-%{version}.tar.gz
# You can also re-generate this tarball from the SCM:
# %{name} refers to the package name and %{version} to the release version 
# git clone git://gitorious.org/colorful-apps/%{name}.git; cd %{name};
# git archive --format=tar --prefix=%{name}-%{version}/ v%{version} | gzip -n > %{name}-%{version}.tar.gz

BuildRequires: pkgconfig(QtCore), pkgconfig(QtGui)
BuildRequires: desktop-file-utils

%description 
%{name} is a simple and good looking memory game that
lays out cards and then you have to find the matching
symbols.
It is a variant of the classic memory game
http://en.wikipedia.org/wiki/Memory_%28game%29

%prep
%setup -q

%build
# This ensures that the files will be placed to the correct location
QMAKEFLAGS=' -after target.path=%{_bindir}'
QMAKEFLAGS+=' -after desktopfile.path=%{_datadir}/applications'
QMAKEFLAGS+=' -after iconfile.path=%{_datadir}/pixmaps'
# This will find qmake on both Fedora and MeeGo
qmake-qt4 $QMAKEFLAGS || qmake $QMAKEFLAGS
make %{?_smp_mflags}

%install
rm -rf $RPM_BUILD_ROOT
make INSTALL_ROOT=$RPM_BUILD_ROOT install
desktop-file-validate %{buildroot}/%{_datadir}/applications/%{name}.desktop

%files
%defattr(-,root,root,-)
%{_bindir}/%{name}
%attr(644,root,root) %{_datadir}/applications/%{name}.desktop
%attr(644,root,root) %{_datadir}/pixmaps/%{name}.png
%doc readme.txt
%doc LICENSE

%changelog
* Tue May 3 2011 Timur Kristóf <venemo@fedoraproject.org> 1.0-1
- Initial version of the RPM package.

