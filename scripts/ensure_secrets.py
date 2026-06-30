import os
Import("env")
sec = os.path.join(env["PROJECT_DIR"], "secrets.h")
ex = os.path.join(env["PROJECT_DIR"], "secrets.h.example")
if not os.path.isfile(sec) and os.path.isfile(ex):
    env.Execute(f'cp "{ex}" "{sec}"')