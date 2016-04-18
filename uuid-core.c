/*
  Copyright (C) 2016 by Syohei YOSHIDA

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#define _BSD_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <emacs-module.h>

#include <uuid.h>

int plugin_is_GPL_compatible;

#define UUID_LEN 37

static emacs_value
Fuuid_generate(emacs_env *env, ptrdiff_t nargs, emacs_value args[], void *data)
{
	emacs_value type = args[0];
	uuid_t uuid;
	char buf[UUID_LEN];

	if (env->eq(env, type, env->intern(env, "nil"))) {
		uuid_generate(uuid);
	} else if (env->eq(env, type, env->intern(env, "random"))) {
		uuid_generate_random(uuid);
	} else if (env->eq(env, type, env->intern(env, "time"))) {
		uuid_generate_time(uuid);
	} else if (env->eq(env, type, env->intern(env, "time-safe"))) {
		uuid_generate_time_safe(uuid);
	} else {
		return env->intern(env, "nil");
	}

	uuid_unparse(uuid, buf);

	return env->make_string(env, buf, (ptrdiff_t)strnlen(buf, UUID_LEN));
}

static void
bind_function(emacs_env *env, const char *name, emacs_value Sfun)
{
	emacs_value Qfset = env->intern(env, "fset");
	emacs_value Qsym = env->intern(env, name);
	emacs_value args[] = { Qsym, Sfun };

	env->funcall(env, Qfset, 2, args);
}

static void
provide(emacs_env *env, const char *feature)
{
	emacs_value Qfeat = env->intern(env, feature);
	emacs_value Qprovide = env->intern (env, "provide");
	emacs_value args[] = { Qfeat };

	env->funcall(env, Qprovide, 1, args);
}

int
emacs_module_init(struct emacs_runtime *ert)
{
	emacs_env *env = ert->get_environment(ert);

#define DEFUN(lsym, csym, amin, amax, doc, data) \
	bind_function (env, lsym, env->make_function(env, amin, amax, csym, doc, data))

	DEFUN("uuid-core-generate", Fuuid_generate, 1, 1, NULL, NULL);

#undef DEFUN

	provide(env, "uuid-core");
	return 0;
}

/*
  Local Variables:
  c-basic-offset: 8
  indent-tabs-mode: t
  End:
*/
