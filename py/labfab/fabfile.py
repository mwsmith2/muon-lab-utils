from fabric.api import run, env, roles

env.use_ssh_config = True
env.roledefs['uwlab'] = ['musun2', 'nmr-daq', 'nmr-cave']
env.roledefs['nmrlab'] = ['nmr-daq', 'nmr-cave']
env.roledefs['muonlab'] = ['musun2', 'musun1']


@roles('uwlab')
def host_type():
    run('uname -s')


@roles('uwlab')
def mkdir(path):
    run('mkdir ' + path)


@roles('uwlab')
def ls(path):
    run('ls ' + path)


@roles('uwlab')
def ln(path1, path2):
    run('ln -s %s %s' % (path1, path2))


def gitclone(url):
    run('mkdir -p ~/Packages')
    run('cd ~/Packages')
    run('git clone %s' % (url))
