pkgname=cxx-run
pkgver=2022.04.14
pkgrel=1
pkgdesc="simple, clean and customisable run dialog in sfml"
arch=('x86_64')
makedepends=('git' 'gcc' 'make' 'sfml')
url="https://github.com/0x7fff-git/arch"
license=('GPL')
source=(git+https://github.com/0x7fff-git/arch)
sha256sums=('SKIP')
provides=('cxx-run')
conflicts=('cxx-run')

pkgver() {
  cd ${pkgname%-git}
  git log -1 --format=format:'%as-%f' | tr - .
}

build() {
  cd ${pkgname%-git}
  ./build
}

package() {
  cd ${pkgname%-git}

  msg2 'Installing executables...'
  install -Dm 755 await -t "$pkgdir"/usr/bin

  msg2 'Cleaning up pkgdir...'
  find "$pkgdir" -type d -name .git -exec rm -r '{}' +
}
