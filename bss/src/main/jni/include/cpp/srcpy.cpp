#include "include/obfuscate.h"

const char *srcpy = O(R"srcpy(
PACKAGE_NAME = "com.netease.newspike"
sync_attrs = {}
REGISTER_UPDATES = []

import sys, traceback, time, os

import inspect

def represent(obj, detailed=True):
    if isinstance(obj, type):
        if obj.__module__ != 'builtins': return f"{obj.__module__}.{obj.__qualname__}"
    else:
        cls = type(obj)
        if getattr(obj, '__module__', 'builtins') != 'builtins' and cls.__repr__ == object.__repr__:
            return f"{(getattr(cls, '__module__', '') + '.') if detailed else ''}{cls.__name__}({hex(id(obj))})"
    
    try: return repr(obj)
    except: return f"{(getattr(cls, '__module__', '') + '.') if detailed else ''}{cls.__name__}({hex(id(obj))})"

def irepr(*args, **kwargs):
    sargs = ([represent(args[0], False)] + [represent(a) for a in args[1:]]) if args else []
    return ', '.join(sargs + [f"{k}={represent(v)}" for k, v in kwargs.items()])

import functools

from typing import Union
def HOOK(clazz, bLog: Union[int, bool] = True, _name=None):
    def decorator(func, _name=_name):
        name = func.__name__
        _name = _name or '_' + name
        
        original = getattr(clazz, name, None)
        # original = getattr(clazz, _name, getattr(clazz, name))
        setattr(clazz, _name, original)
        
        import functools
        @functools.wraps(original)
        def wrapper(*args, **kwargs):
            try:
                if bLog: print(f"HENTR: {clazz.__name__}.{name}({irepr(*args, **kwargs)})")
                
                # if not inspect.ismethod(original) and inspect.isfunction(original): result = func(clazz, *args, **kwargs)
                result = func(*args, **kwargs)
                
                if bLog: print(f"HEXIT: [bold magenta]{clazz.__name__}.{name}[/bold magenta] -> {represent(result)}")
                return result
            except: print(__import__('traceback').format_exc())
        
        setattr(clazz, name, wrapper)
        return func
    return decorator

def TRY(_func=None, *, bLog=False):
    def decorator(original):
        @functools.wraps(original)
        def wrapper(*args, **kwargs):
            try:
                if bLog: print(f"TRY: {original.__name__}({irepr(args, kwargs)})")
                
                result = original(*args, **kwargs)
                
                if bLog: print(f"TRY: [bold magenta]{original.__name__}[/bold magenta] -> {represent(result)}")
                return result
            except Exception: print(__import__('traceback').format_exc())
        return wrapper
    return decorator(_func) if _func else decorator

def HOOKO(clazz, name, replace = None, log = False):
    if not replace: replace = lambda *_, **__: None
    
    _name = '_' + name
    
    func = getattr(clazz, name)
    setattr(clazz, _name, func)
    def _(*_, **__):
        try:
            if log: print(f"LOG: {clazz.__name__}.{name}({irepr(*args, **kwargs)})")
            return replace(*_, **__)
        except: print(__import__('traceback').format_exc())
    setattr(clazz, name, _)

def LOGC(clazz, exclude_methods=None):
    import inspect, types
    from functools import wraps
    
    def represent(obj, detailed=True):
        if isinstance(obj, type):
            if obj.__module__ != 'builtins': return f"{obj.__module__}.{obj.__qualname__}"
        else:
            cls = type(obj)
            if getattr(obj, '__module__', 'builtins') != 'builtins' and cls.__repr__ == object.__repr__:
                return f"{(getattr(cls, '__module__', '') + '.') if detailed else ''}{cls.__name__}({hex(id(obj))})"
        
        try: return repr(obj)
        except: return f"{(getattr(cls, '__module__', '') + '.') if detailed else ''}{cls.__name__}({hex(id(obj))})"

    exclude_methods = set(exclude_methods or ''.split('.'))
    
    for name in clazz.__dict__:
        if name in exclude_methods or (name != '__init__' and name.startswith('__')): continue

        attr = clazz.__dict__[name]

        if not isinstance(attr, (classmethod, staticmethod, types.FunctionType)): continue

        original_func = attr.__func__ if isinstance(attr, (classmethod, staticmethod)) else attr
        cm = isinstance(attr, classmethod)
        sm = isinstance(attr, staticmethod)
        is_async = inspect.iscoroutinefunction(original_func)
        
        def create_wrapper(func, name, cm, sm, is_async):
            @wraps(func)
            def sync_wrapper(*args, **kwargs):
                try:
                    bNone = set(''.split('.'))
                    bFalse = set(''.split('.'))
                    bTrue = set(''.split('.'))
                    bLog = name not in set(''.split('.')) | bFalse | bNone
                    
                    if bLog: print(f"ENTR: {clazz.__name__}.{name}({irepr(*args, **kwargs)})")
                    '''
                    if name == 'CheckCostAmmo': result = func(args[0], args[1], False)
                    elif name in bNone: result = None
                    elif name in bFalse: result = False
                    elif name in bTrue: result = False'''
                    result = func(*args, **kwargs)
                    
                    if bLog: print(f"EXIT: [bold magenta]{clazz.__name__}.{name}[/bold magenta] -> {represent(result)}")
                    return result
                except Exception as e:
                    print(traceback.format_exc())
                    print(f"ERRR in {clazz.__name__}.{name}: {str(e)}")
                    raise

            @wraps(func)
            async def async_wrapper(*args, **kwargs):
                try:
                    print(f"ENTR_ASYNC: {clazz.__name__}.{name}({irepr(*args, **kwargs)})")
                    result = await func(*args, **kwargs)
                    print(f"EXIT_ASYNC: [bold magenta]{clazz.__name__}.{name}[/bold magenta] -> {represent(result)}")
                    return result
                except Exception as e:
                    print(traceback.format_exc())
                    print(f"ERRR_ASYNC in {clazz.__name__}.{name}: {str(e)}")
                    raise

            return async_wrapper if is_async else sync_wrapper

        wrapper = create_wrapper(original_func, name, cm, sm, is_async)

        if cm: setattr(clazz, name, classmethod(wrapper))
        elif sm: setattr(clazz, name, staticmethod(wrapper))
        else: setattr(clazz, name, wrapper)

def LOGM(module, exclude_clazz=None, blab=False):
    import inspect
    exclude_clazz = set(exclude_clazz or 'Singleton.enum.StoryTick'.split('.'))
    for name, clazz in inspect.getmembers(module, inspect.isclass):
        if name in exclude_clazz: continue
        if blab:
            if clazz.__module__ != getattr(module, '__name__', clazz.__module__): continue
        LOGC(clazz)

def LOG(obj):
    LOGM(obj)
    LOGC(obj)

from gclient.framework.entities.space import Space

def WorldToScreenPoint(worldpos):
    return Space._instance.camera.engine_camera.GetScreenPointFromWorldPoint(worldpos)

def IsVisible(local, target, targetId, distance=50):
    raycast = Space._instance.RawRaycast(local, distance, 19, with_trigger=False, to_pos=target)

    ownerid = getattr(raycast.Body, 'ownerid', None)
    # print(f"{ownerid}, {targetId}")
    if raycast.Body and ownerid:
        if ownerid == targetId: return True
    elif raycast.Flags == 7: return True

    return False

def igetattr(*_, **__):
    try: return getattr(*_, **__)
    except: return 'igetattr.error'

import inspect
def idir(obj): return {k: igetattr(obj, k, 'undefined') for k in dir(obj)}

def stacktrace():
    for frame_info in reversed(inspect.stack()):
        print(f"File: {frame_info.filename}, Line: {frame_info.lineno}, Function: {frame_info.function}")

def trace_handler(frame, event, arg):
    if event == 'call':
        with open('kk.log', 'a') as w:
            w.write(f'Calling function: {frame.f_code.co_name} at line {frame.f_lineno}\n')
    elif event == 'line':
        with open('kk.log', 'a') as a:
            a.write(f'Executing line {frame.f_lineno} in {frame.f_code.co_name}\n')
    elif event == 'return':
        with open('kk.log', 'a') as a:
            a.write(f'Returning from function: {frame.f_code.co_name} with value {arg}\n')
    return trace_handler

import random
def trueChance(probability): return random.random() < probability

def randomHitOffset():
    return random.uniform(0.05, 0.1) * random.choice([1, -1])

def V3Minus(a, b): return (a.x - b.x, a.y - b.y, a.z - b.z)
def V32T(v):
    try: return v.x, v.y, v.z
    except: return v[0], v[1], v[2]

def normalize(x, y, z):
    mag = (x**2 + y**2 + z**2) ** 0.5
    epsilon = 1e-8
    
    if mag < epsilon: return (x, y, z)
    
    return (x/mag, y/mag, z/mag)

def Distance(a, b):
    dx = b[0] - a[0]
    dy = b[1] - a[1]
    dz = b[2] - a[2]
    return (dx**2 + dy**2 + dz**2) ** 0.5

def v2Distance(a, b):
    dx = b[0] - a[0]
    dy = b[1] - a[1]
    return (dx**2 + dy**2) ** 0.5

from gshare.formula import Distance2D, Normalize2D
from gshare.formula import Distance3D, Normalize3D

class Vector2:
    def __init__(self, x: float = 0, y: float = 0):
        self.x = float(x)
        self.y = float(y)
    
    def __getitem__(self, key):
        if key in (0, -2): return self.x
        elif key in (1, -1): return self.y
        else: raise IndexError(f"Vector2 index out of range {key=}")
    
    def __setitem__(self, key, value):
        if key in (0, -2): self.x = float(value)
        elif key in (1, -1): self.y = float(value)
        else: raise IndexError(f"Vector2 index out of range {key=}")
    
    def __repr__(self): return f"Vector2({self.x}, {self.y})"
    
    def __add__(self, other):
        if isinstance(other, Vector2): return Vector2(self.x + other.x, self.y + other.y)
        elif isinstance(other, tuple) and len(other) == 2: return Vector2(self.x + other[0], self.y + other[1])
        elif isinstance(other, (int, float)): return Vector2(self.x + other, self.y + other)
        else: raise TypeError(f"Cannot add Vector2 and {type(other)}")
    def __radd__(self, other): return self.__add__(other)

    def __sub__(self, other):
        if isinstance(other, Vector2): return Vector2(self.x - other.x, self.y - other.y)
        elif isinstance(other, tuple) and len(other) == 2: return Vector2(self.x - other[0], self.y - other[1])
        elif isinstance(other, (int, float)): return Vector2(self.x - other, self.y - other)
        else: raise TypeError(f"Cannot subtract {type(other)} from Vector2")

    def __rsub__(self, other):
        if isinstance(other, tuple) and len(other) == 2: return Vector2(other[0] - self.x, other[1] - self.y)
        elif isinstance(other, (int, float)): return Vector2(other - self.x, other - self.y)
        else: raise TypeError(f"Cannot subtract Vector2 from {type(other)}")

    def __mul__(self, other):
        if isinstance(other, Vector2): return Vector2(self.x * other.x, self.y * other.y)
        elif isinstance(other, tuple) and len(other) == 2: return Vector2(self.x * other[0], self.y * other[1])
        elif isinstance(other, (int, float)): return Vector2(self.x * other, self.y * other)
        else: raise TypeError(f"Cannot multiply Vector2 and {type(other)}")
    def __rmul__(self, other): return self.__mul__(other)
    
    def __truediv__(self, other):
        if isinstance(other, Vector2):
            if other.x == 0 or other.y == 0: raise ZeroDivisionError("Division by zero in Vector2")
            return Vector2(self.x / other.x, self.y / other.y)
        elif isinstance(other, tuple) and len(other) == 2:
            if 0 in other: raise ZeroDivisionError("Division by zero in Vector2")
            return Vector2(self.x / other[0], self.y / other[1])
        elif isinstance(other, (int, float)):
            if other == 0: raise ZeroDivisionError("Division by zero in Vector2")
            return Vector2(self.x / other, self.y / other)
        else: raise TypeError(f"Cannot divide Vector2 by {type(other)}")

    def __rtruediv__(self, other):
        if isinstance(other, tuple) and len(other) == 2:
            if self.x == 0 or self.y == 0: raise ZeroDivisionError("Division by zero in Vector2")
            return Vector2(other[0] / self.x, other[1] / self.y)
        elif isinstance(other, (int, float)):
            if self.x == 0 or self.y == 0: raise ZeroDivisionError("Division by zero in Vector2")
            return Vector2(other / self.x, other / self.y)
        else: raise TypeError(f"Cannot divide {type(other)} by Vector2")

    def __neg__(self): return Vector2(-self.x, -self.y)
    
    def __eq__(self, other):
        if not isinstance(other, Vector2): return False
        return self.x == other.x and self.y == other.y
    def __ne__(self, other): return not self.__eq__(other)
    
    def __iter__(self):
        yield self.x
        yield self.y

class Vector3:
    def __init__(self, x: float = 0, y: float = 0, z: float = 0):
        self.x = float(x)
        self.y = float(y)
        self.z = float(z)
    
    def __getitem__(self, key):
        if key in (0, -3): return self.x
        elif key in (1, -2): return self.y
        elif key in (2, -1): return self.z
        else: raise IndexError("Vector3 index out of range")
    
    def __setitem__(self, key, value):
        if key in (0, -3): self.x = float(value)
        elif key in (1, -2): self.y = float(value)
        elif key in (2, -1): self.z = float(value)
        else: raise IndexError("Vector3 index out of range")
    
    def __repr__(self): return f"Vector3({self.x}, {self.y}, {self.z})"
    
    def __add__(self, other):
        if isinstance(other, Vector3): return Vector3(self.x + other.x, self.y + other.y, self.z + other.z)
        elif isinstance(other, tuple) and len(other) == 3: return Vector3(self.x + other[0], self.y + other[1], self.z + other[2])
        elif isinstance(other, (int, float)): return Vector3(self.x + other, self.y + other, self.z + other)
        else: raise TypeError(f"Cannot add Vector3 and {type(other)}")
    def __radd__(self, other): return self.__add__(other)

    def __sub__(self, other):
        if isinstance(other, Vector3): return Vector3(self.x - other.x, self.y - other.y, self.z - other.z)
        elif isinstance(other, tuple) and len(other) == 3: return Vector3(self.x - other[0], self.y - other[1], self.z - other[2])
        elif isinstance(other, (int, float)): return Vector3(self.x - other, self.y - other, self.z - other)
        else: raise TypeError(f"Cannot subtract {type(other)} from Vector3")

    def __rsub__(self, other):
        if isinstance(other, tuple) and len(other) == 3: return Vector3(other[0] - self.x, other[1] - self.y, other[2] - self.z)
        elif isinstance(other, (int, float)): return Vector3(other - self.x, other - self.y, other - self.z)
        else: raise TypeError(f"Cannot subtract Vector3 from {type(other)}")

    def __mul__(self, other):
        if isinstance(other, Vector3): return Vector3(self.x * other.x, self.y * other.y, self.z * other.z)
        elif isinstance(other, tuple) and len(other) == 3: return Vector3(self.x * other[0], self.y * other[1], self.z * other[2])
        elif isinstance(other, (int, float)): return Vector3(self.x * other, self.y * other, self.z * other)
        else: raise TypeError(f"Cannot multiply Vector3 and {type(other)}")
    def __rmul__(self, other): return self.__mul__(other)
    
    def __truediv__(self, other):
        if isinstance(other, Vector3):
            if other.x == 0 or other.y == 0 or other.z == 0: raise ZeroDivisionError("Division by zero in Vector3")
            return Vector3(self.x / other.x, self.y / other.y, self.z / other.z)
        elif isinstance(other, tuple) and len(other) == 3:
            if 0 in other: raise ZeroDivisionError("Division by zero in Vector3")
            return Vector3(self.x / other[0], self.y / other[1], self.z / other[2])
        elif isinstance(other, (int, float)):
            if other == 0: raise ZeroDivisionError("Division by zero in Vector3")
            return Vector3(self.x / other, self.y / other, self.z / other)
        else: raise TypeError(f"Cannot divide Vector3 by {type(other)}")

    def __rtruediv__(self, other):
        if isinstance(other, tuple) and len(other) == 3:
            if self.x == 0 or self.y == 0 or self.z == 0: raise ZeroDivisionError("Division by zero in Vector3")
            return Vector3(other[0] / self.x, other[1] / self.y, other[2] / self.z)
        elif isinstance(other, (int, float)):
            if self.x == 0 or self.y == 0 or self.z == 0: raise ZeroDivisionError("Division by zero in Vector3")
            return Vector3(other / self.x, other / self.y, other / self.z)
        else: raise TypeError(f"Cannot divide {type(other)} by Vector3")

    def __neg__(self): return Vector3(-self.x, -self.y, -self.z)
    
    def __eq__(self, other):
        if not isinstance(other, Vector3): return False
        return self.x == other.x and self.y == other.y and self.z == other.z
    def __ne__(self, other): return not self.__eq__(other)
    
    def __iter__(self):
        yield self.x
        yield self.y
        yield self.z

import fcntl
import json
import os
import time
import threading


SYNC_FIFO_PATH = "/data/data/" + PACKAGE_NAME + "/imgui/sync"
if not os.path.exists(SYNC_FIFO_PATH): os.mkfifo(SYNC_FIFO_PATH, 0o666)

def read_fifo():
    fifo = None
    try:
        fifo = open(SYNC_FIFO_PATH, "r")
        while True:
            line = fifo.readline()
            if line:
                line = line.strip('\r\n')
                if not line: continue
                try:
                    sync_attrs.update(json.loads(line))
                    # print(json.dumps(checkbox_dict, indent=2))
                except json.JSONDecodeError as e:
                    print(f"Error decoding JSON: {e}")
                    print(f"Problematic JSON string: {line}")
            else: time.sleep(0.1)
    except Exception as e:
        print(f"Error with FIFO: {e}")
        time.sleep(0.5)
    finally:
        if fifo: fifo.close()

fifo_thread = threading.Thread(target=read_fifo, daemon=True)
fifo_thread.start()

from gclient.ui.uilogin.login_window import LoginWindow
from gclient.framework.util.story_tick import StoryTick

_register_lock = threading.Lock()
_last_story_tick_id = None
_delayed_register_active = False

def _register_updates():
    global _last_story_tick_id
    with _register_lock:
        try:
            st = StoryTick._instance
            if not st:
                return False
            current_id = id(st)
            if _last_story_tick_id == current_id:
                return True
            for update in REGISTER_UPDATES:
                st.Add(update, 120)
            _last_story_tick_id = current_id
            print("[MOD] Updates registered to StoryTick " + str(current_id))
            return True
        except Exception:
            print(__import__('traceback').format_exc())
            return False

@HOOK(LoginWindow, 0)
def TryAutoEnterGame(self):
    _register_updates()
    self.OnEnterGameClick()

from gclient.framework.entities.space import Space as _SpaceForHook

@HOOK(_SpaceForHook, 0)
def __init__(self, *args, **kwargs):
    global _delayed_register_active
    result = self.___init__(*args, **kwargs)
    print("[MOD] Space.__init__ detected, will re-register updates")

    if _delayed_register_active:
        return result

    _delayed_register_active = True

    def _delayed_register():
        global _delayed_register_active
        import time as _time
        try:
            for i in range(30):
                _time.sleep(1)
                try:
                    st = StoryTick._instance
                    if st and id(st) != _last_story_tick_id:
                        if _register_updates():
                            print("[MOD] Updates re-registered after Space init (attempt " + str(i+1) + ")")
                            return
                except Exception:
                    pass
            print("[MOD] WARNING: Failed to re-register updates after 30 attempts")
        finally:
            _delayed_register_active = False

    _t = threading.Thread(target=_delayed_register, daemon=True)
    _t.start()
    return result

def _space_entity_watcher():
    import time as _time
    print("[MOD] Entity watcher thread started")
    while True:
        try:
            _time.sleep(3)
            st = StoryTick._instance
            if not st:
                continue
            if id(st) == _last_story_tick_id:
                continue
            sp = getattr(Space, '_instance', None)
            if sp and getattr(sp, 'entities', None) and len(sp.entities) > 0:
                if _register_updates():
                    print("[MOD] Watcher: Updates registered after entities detected")
        except Exception:
            pass

_watcher_thread = threading.Thread(target=_space_entity_watcher, daemon=True)
_watcher_thread.start()

import os, struct, json

from gclient.framework.util.story_tick import StoryTick

from gclient.gameplay.logic_base.entities.combat_avatar import CombatAvatar

ENTITY_FIFO_PATH = "/data/data/" + PACKAGE_NAME + "/imgui/esync"
if not os.path.exists(ENTITY_FIFO_PATH): os.mkfifo(ENTITY_FIFO_PATH)

def TOT2(x): return x[0], x[1]

@TRY
def SafeScreenPoint(world_pos):
    screen_pos = WorldToScreenPoint(world_pos)
    if not screen_pos or screen_pos.z <= 0:
        return None
    return TOT2(screen_pos)
 
    
def EspUpdate(*_, **__):
    inst = genv.space
    if not isinstance(inst, Space): return
    if not inst.entities: return
    if not genv.avatar: return

    localEntity = genv.avatar.combat_avatar
    if not localEntity or not hasattr(localEntity, 'model'): return

    if sync_attrs.get('bSpeed'):
        localEntity.model.SetMoveSpeed(sync_attrs.get('fSpeed', 1) + 7)

    entities = []  # reset tiap frame

    for entityId, entity in inst.entities.copy().items():
        if not isinstance(entity, CombatAvatar): continue
        if not getattr(entity, 'model', None): continue
        if not entity.is_alive: continue
        if entity == localEntity: continue
        if is_same_team(entity, localEntity): continue

        # X-Ray mode
        if sync_attrs.get('bXray'):
            entity.model.UseTechHighLightXray(
                param=(1, 0, 0),
                param2=(0, 0, 0, 1),
                color2=(1, 0, 0, 1)
            )

        head_world = entity.model.GetBoneWorldPosition('biped Head')
        if not head_world: continue

        head_screen = WorldToScreenPoint(head_world)
        if not head_screen or head_screen.z <= 0: continue

        # Tidak skip kalau off-screen
        is_on_screen = True  

        bones = {
            "head": SafeScreenPoint(entity.model.GetBoneWorldPosition('biped Head')),
            "spine": SafeScreenPoint(entity.model.GetBoneWorldPosition('biped Spine')),
            "spine1": SafeScreenPoint(entity.model.GetBoneWorldPosition('biped Spine1')),
            "limbs_r_upperarm": SafeScreenPoint(entity.model.GetBoneWorldPosition('biped R UpperArm')),
            "limbs_l_upperarm": SafeScreenPoint(entity.model.GetBoneWorldPosition('biped L UpperArm')),
            "limbs_r_forearm": SafeScreenPoint(entity.model.GetBoneWorldPosition('biped R Forearm')),
            "limbs_l_forearm": SafeScreenPoint(entity.model.GetBoneWorldPosition('biped L Forearm')),
            "limbs_r_hand": SafeScreenPoint(entity.model.GetBoneWorldPosition('biped R Hand')),
            "limbs_l_hand": SafeScreenPoint(entity.model.GetBoneWorldPosition('biped L Hand')),
            "limbs_r_thigh": SafeScreenPoint(entity.model.GetBoneWorldPosition('biped R Thigh')),
            "limbs_l_thigh": SafeScreenPoint(entity.model.GetBoneWorldPosition('biped L Thigh')),
            "limbs_r_calf": SafeScreenPoint(entity.model.GetBoneWorldPosition('biped R Calf')),
            "limbs_l_calf": SafeScreenPoint(entity.model.GetBoneWorldPosition('biped L Calf')),
            "limbs_r_foot": SafeScreenPoint(entity.model.GetBoneWorldPosition('biped R Foot')),
            "limbs_l_foot": SafeScreenPoint(entity.model.GetBoneWorldPosition('biped L Foot')),
            "limbs_r_toe": SafeScreenPoint(entity.model.GetBoneWorldPosition('biped R Toe')),
            "limbs_l_toe": SafeScreenPoint(entity.model.GetBoneWorldPosition('biped L Toe')),
        }

        if any(v is None for v in bones.values()):
            continue

        entities.append({
            "is_on_screen": is_on_screen,
            "bones": bones,
            "is_bot": entity.IsRobotCombatAvatar,
            "is_knocked": entity.is_dying_state,
            "is_visible": IsVisible(Space._instance.camera.position, head_world, entity.id),
            "is_health": float(getattr(entity, 'hp', 0.0)),
            "max_health": float(getattr(entity, 'cur_maxhp', 125.0)),
            "is_armor": float(getattr(entity, 'armor', 0.0)) + float(getattr(entity, 'extra_armor', 0.0)),
            "max_armor": float(getattr(entity, 'base_maxarmor', 100.0)),
            "team_id": float(getattr(entity, "team", 0)),
            "entity_name": str(getattr(entity, "name", "")),
        })

    # Limit 20 entity
    entities = entities[:20]

    # Kirim data via FIFO tanpa blocking
    import errno
    try:
        fd = os.open(ENTITY_FIFO_PATH, os.O_WRONLY | os.O_NONBLOCK)
        os.write(fd, (json.dumps(entities) + '\n').encode())
        os.close(fd)
    except OSError as e:
        if e.errno != errno.EAGAIN:
            raise


REGISTER_UPDATES.append(EspUpdate)

from gclient.gameplay.logic_base.entities.combat_avatar import CombatAvatar

from gclient.framework.util.gyroscope import GyroscopePoseCombat

from typing import Optional

from gclient.gameplay.logic_base.equips.weapon_case import WeaponCase
from gclient.gameplay.logic_base.entities.combatavatarmembers import cimp_anti_plugin


LOG_FILE = "/sdcard/bypasslog.txt"

def log_bypass(text):
    try:
        with open(LOG_FILE, "a") as f:
            f.write(text + "\n")
    except Exception as e:
        print(f"[LOG ERROR] {e}")
                
                
# ✅ Hook ke ProcessNativeHotfix (index 2)
@HOOK(cimp_anti_plugin.PlayerCombatAvatarMember, 2)
def ProcessNativeHotfix(self):
    if sync_attrs.get('bBypassPlugin'):
        print("[BYPASS] ProcessNativeHotfix diblok otomatis")
        log_bypass("[BYPASS] ProcessNativeHotfix() diblok")
        return None
    return self._ProcessNativeHotfix()

# ✅ Hook ke AddNativeHotfix (index 0)
@HOOK(cimp_anti_plugin.PlayerCombatAvatarMember, 0)
def AddNativeHotfix(self, content):
    if sync_attrs.get('bBypassPlugin'):
        print("[BYPASS] AddNativeHotfix dibatalkan otomatis")
        log_bypass("[BYPASS] AddNativeHotfix() dibatalkan")
        return
    return self._AddNativeHotfix(content)

# ✅ Hook ke AntiPenetrateDrawMovePath (index 1)
# NOTE: This function MUST always call through to the original.
# It sends bullet trajectory data to the server for hit validation in ranked/online modes.
# Blocking it causes the server to receive no path data, marking all hits as invalid
# and applying near-zero "fake damage" — even with aimbot/ESP disabled.
@HOOK(cimp_anti_plugin.PlayerCombatAvatarMember, 1)
def AntiPenetrateDrawMovePath(self, pivots, hits):
    return self._AntiPenetrateDrawMovePath(pivots, hits)
        
        
@HOOK(WeaponCase)
def SetSwitchWeaponDuration(self, raise_speed, drop_speed, **__):
    return self._SetSwitchWeaponDuration(self, raise_speed, drop_speed, **__) * (0.0001 if sync_attrs.get('bSwitch') else 1)

@HOOK(WeaponCase, False)
def GetRealRecoilData(self, is_real_ads, shoot_idx):
    return [x * (0.0001 if sync_attrs.get('bNoRecoil') else 1) for x in self._GetRealRecoilData(is_real_ads, shoot_idx)]

@HOOK(WeaponCase)
def GetRealShootScatterRadian(self, shoot_idx, **__):
    return self._GetRealShootScatterRadian(shoot_idx, **__) * (0.0001 if sync_attrs.get('bNoSpread') else 1)

from gclient.framework.camera.fps_placer import FpsPlacer

FpsPlacerInstance: Optional[FpsPlacer] = None
@HOOK(FpsPlacer, 0)
def OnStaticTick(self, *_, **__):
    global FpsPlacerInstance
    if self: FpsPlacerInstance = self

@TRY


def get_team_id(entity):
    return getattr(entity, 'combat_team', None)

def is_same_team(entity1, entity2):
    team1 = get_team_id(entity1)
    team2 = get_team_id(entity2)
    if team1 is None or team2 is None:
        return False
    return team1 == team2

def AIMUpdate(*_, **__):
    if not sync_attrs.get('bAIM'): return
    if not genv.avatar: return

    localEntity = genv.avatar.combat_avatar
    if not localEntity: return

    if not FpsPlacerInstance:
        print('noFpsPlacerInstance')
        return

    curWeapon = localEntity.GetCurWeapon()
    if not curWeapon: return

    targetEntity = targetEntityPos = targetEntityPosx = None
    targetDistance = sync_attrs.get('fAIM_Fov', 1000)

    for entityId, entity in Space._instance.entities.copy().items():
        if not isinstance(entity, CombatAvatar): continue
        if not entity.is_alive: continue
        if entity == localEntity: continue
        if is_same_team(entity, localEntity): continue

        head = entity.model.GetBoneWorldPosition('biped Head')
        if not head: continue

        if sync_attrs.get('bAIM_CheckVisibility', True):
            if not IsVisible(Space._instance.camera.position, head, entity.id):
                continue

        # Pilih bone target
        bone_index = sync_attrs.get('iAIM_TargetBone', 0)
        match bone_index:
            case 0: target_bone = "biped Head"
            case 1: target_bone = "biped Spine2"
            case 2: target_bone = "biped L Hand"
            case 3: target_bone = "biped R Hand"
            case 4: target_bone = "biped L Foot"
            case 5: target_bone = "biped R Foot"
            case 6: target_bone = "biped Pelvis"
            case _: target_bone = "biped Head"

        target_world_pos = entity.model.GetBoneWorldPosition(target_bone)
        entityPosx = WorldToScreenPoint(target_world_pos)
        if not entityPosx or entityPosx.z <= 0.0: continue

        distance = Distance2D(
            (entityPosx.x, entityPosx.y),
            (sync_attrs.get('fWidth', 2400) * 0.5, sync_attrs.get('fHeight', 1080) * 0.5)
        )

        if distance < targetDistance:
            targetDistance = distance
            targetEntity, targetEntityPos, targetEntityPosx = entity, target_world_pos, entityPosx

    # ❗ Reset jika tidak ada target
    if not targetEntity:
        try:
            with open("/data/data/" + PACKAGE_NAME + "/imgui/aim_target", "w") as f:
                f.write(json.dumps({
                    "valid": False,
                    "visible": False
                }) + "\n")
        except: pass
        return

    if sync_attrs.get('bAIM_IgnoreBots', False) and targetEntity.IsRobotCombatAvatar: return
    if sync_attrs.get('bAIM_IgnoreKnocked', True) and targetEntity.is_dying_state: return

    # ✅ Update target info ke file untuk ImGui
    try:
        target_info = {
            "x": targetEntityPosx.x,
            "y": targetEntityPosx.y,
            "valid": True,
            "visible": True if not sync_attrs.get('bAIM_CheckVisibility', True) else IsVisible(
                Space._instance.camera.position, targetEntityPos, targetEntity.id
            )
        }
        with open("/data/data/" + PACKAGE_NAME + "/imgui/aim_target", "w") as f:
            f.write(json.dumps(target_info) + "\n")
    except Exception as e:
        print("Write aim_target failed:", e)

    # Trigger aimbot
    match sync_attrs.get('iAIM_Trigger', 0):
        case 0:
            if not localEntity.is_shooting and not localEntity.is_ads: return
        case 1:
            if not localEntity.is_ads: return
        case 2:
            if not localEntity.is_shooting: return
        case 3:
            pass

    Screen3_2 = Vector2(
        sync_attrs.get('fWidth', 2400) * 0.5,
        sync_attrs.get('fHeight', 1080) * 0.5
    )
    hitDir = Vector2(targetEntityPosx.x - Screen3_2.x, targetEntityPosx.y - Screen3_2.y)
    if abs(hitDir[0]) < 0.00001 and abs(hitDir[1]) < 0.00001: return

    hitDir = Normalize2D(hitDir)

    fov = Space._instance.camera.engine_camera.FieldOfView if Space._instance and Space._instance.camera else 90.0
    zoom_factor = max(1.0, 90.0 / fov)

    base_snap = sync_attrs.get('fAIM_SnapStrength', 0.1)
    smoothing = 0.001 * base_snap / zoom_factor

    x = hitDir[0] * targetDistance * smoothing
    y = hitDir[1] * targetDistance * smoothing

    max_delta = max(1.0, 5.0 / zoom_factor)
    x = max(-max_delta, min(max_delta, x))
    y = max(-max_delta, min(max_delta, y))

    if abs(x + y) > 0.00001:
        FpsPlacerInstance.TurnView(x, -y)
        
REGISTER_UPDATES.append(AIMUpdate)

from gclient.framework.entities.space import Space
from gclient.gameplay.logic_base.entities.combat_avatar import PlayerCombatAvatar


BONES = {
    'Head': {
        'name': 'Head',
        'scale': (1.1000000124848068, 1.099999922508427, 1.0999998395187762), # CHANGES DEPENDING ON HERO/SKIN
        'bone_name': 'biped Head',
        'dmg_name': 'head_damage'
    },
    'UpperTop': {
        'name': 'UpperTop',
        'scale': (1.100007990904212, 1.1001234035373453, 1.1001123001942972),
        'bone_name': 'biped Spine',
        'dmg_name': 'uppertop_damage'
    },
}

from gclient.gameplay.logic_base.entities.cdrone import DRONE_CAMERA_OFFSET
Vector3 = type(DRONE_CAMERA_OFFSET)

def GetHitData(wp, entity, bone):
    startPos = Space._instance.camera.position
    bonePos = entity.model.GetBoneWorldPosition(bone['bone_name'])
    
    for _ in range(30):
        hitOffset = Vector3(randomHitOffset(), randomHitOffset(), randomHitOffset())
        hitPos = bonePos + hitOffset
        
        hitDir, distance = DirAndLen3D(startPos, hitPos)
        if distance > wp['damage_range']: break
        
        raycast = Space._instance.RawRaycast(startPos, wp['damage_range'], 19, with_trigger=False, to_pos=hitPos)
        
        if raycast.Body and getattr(raycast.Body, 'ownerid', None):
            if getattr(raycast.Body, 'ownerid', None) == entity.id: return raycast, distance, startPos, hitDir, hitPos, hitOffset
        else:
            if raycast.Flags == 7: return raycast, distance, startPos, hitDir, hitPos, hitOffset
    
    return 0, 0, 0, 0, 0, 0

from gshare.formula import DirAndLen3D

def dropDamage(wp, dmg, distance):
    if distance > wp['damage_range']: return 0
    if distance < wp['damage_range_1']: return dmg
    if distance < wp['damage_range_2']: return dmg * wp['damage_dropoff_1']
    if distance < wp['damage_range_3']: return dmg * wp['damage_dropoff_2']
    return dmg * wp['damage_dropoff_3']

def GetTargetEntity(maxDist = None):
    targetEntity, nearestDistance = None, maxDist or float('inf')
    
    for entityId, entity in Space._instance.entities.copy().items():
        if not isinstance(entity, CombatAvatar): continue
        if not entity.is_alive: continue
        
        entityPosx = WorldToScreenPoint(Vector3(*entity.position))
        if entityPosx.z <= 0.0: continue
        
        # if not IsVisible(Space._instance.camera.position, entity.model.GetBoneWorldPosition('biped Head')): continue
        
        distance = Distance2D((entityPosx.x, entityPosx.y), (sync_attrs.get('fWidth', 2400) * 0.5, sync_attrs.get('fHeight', 1080) * 0.5))
        if distance < nearestDistance:
            nearestDistance = distance
            targetEntity = entity
    
    return targetEntity

import time


@HOOK(PlayerCombatAvatar, 0)
def CallServerGameLogic(self, method, *args, **kwargs):
    if sync_attrs.get('bBulletTrack') and method == 'BatchDealSpellResult' and Space._instance.entities:
        for hit in args[0].copy():
            if 'caster' not in hit: continue
            
            localEntity = genv.avatar.combat_avatar
            if not localEntity: continue
            if localEntity.id != hit['caster']: continue
            
            probability = sync_attrs.get('fBulletTrack_Probability', 70) / 100
            if 'damage_result' in hit:
                if not trueChance(probability): continue
                
                # print(f"ENTER: HEAD({irepr(*args, **kwargs)})")
                
                for _, hitData in hit['damage_result'].items():
                    targetId = hitData.get('target_id', None)
                    
                    if hitData.get('hit_part', 'Head') == 'Head': continue
                    
                    if not hitData.get('weapon_guid'): print(f"NO.HIT.DATA.WEAPON.GUID({irepr(*args, **kwargs)})")
                    
                    weapon_guid = hitData.get('weapon_guid') or hit.get('weapon_guid')
                    if not weapon_guid: continue
                    
                    curWeapon = localEntity.GetWeaponByGuid(weapon_guid)
                    if not curWeapon: continue
                    curWeaponProto = curWeapon.weapon_proto
                    
                    hitEntity = genv.space.entities.get(_, None)
                    if not hitEntity: continue
                    if not isinstance(hitEntity, CombatAvatar): continue
                    if not hitEntity.is_alive: continue
                    if sync_attrs.get('bBulletTrack_IgnoreBots') and hitEntity.IsRobotCombatAvatar: continue

                    bone = BONES['Head']
                    
                    raycast, distance, startPos, hitDir, hitPos, hitOffset = GetHitData(curWeaponProto, hitEntity, bone)
                    if not raycast: continue
                    
                    damage = float(curWeaponProto[bone['dmg_name']])
                    damage = dropDamage(curWeaponProto, damage, distance)
                    if not damage: continue
                    
                    # print(f"head delt {damage}")
                    
                    hitData['hit_dir'] = hitDir
                    hitData['hit_pos'] = V32T(hitPos)
                    hitData['hit_part'] = bone['name']
                    hitData['damage'] = damage
                    hitData['verify_hit_offset'] = V32T(hitOffset)
                    
                    for hitEffect in hit.get('effect_id', ()):
                        hitEffect['hit_pos'] = hitData['hit_pos']
                        hitEffect['hit_normal'] = V32T(Space._instance.RawRaycast(Space._instance.camera.position, 300, 19, with_trigger=False, to_pos=hitPos).Normal)
                        hitEffect['hit_dir'] = hitData['hit_dir']
                
                # print(f"EXIIT: HEAD({irepr(*args, **kwargs)})")
            elif 'ballistic_effect' in hit or ('sound_result' in hit and 'make_ballistick_effect' in hit.get('extra', ())):
                # print(f"ENTER: FORCE({irepr(*args, **kwargs)})")
                
                if localEntity.is_ads: probability += 0.10
                if not trueChance(probability): continue
                
                curWeapon = localEntity.GetCurWeapon()
                if not curWeapon: continue
                curWeaponProto = curWeapon.weapon_proto
                
                targetEntity = GetTargetEntity(sync_attrs.get('fAIM_Fov'))
                if not targetEntity: continue
                if sync_attrs.get('bBulletTrack_IgnoreBots') and targetEntity.IsRobotCombatAvatar: continue
                if sync_attrs.get('bBulletTrack_IgnoreKnocked') and targetEntity.is_dying_state: continue
                
                bone = BONES['Head']
                
                raycast, distance, startPos, hitDir, hitPos, hitOffset = GetHitData(curWeaponProto, targetEntity, bone)
                if not raycast: continue
                
                damage = float(curWeaponProto[bone['dmg_name']])
                damage = dropDamage(curWeaponProto, damage, distance)
                if not damage: continue
                
                # print(f"force delt {damage}")
                
                hitEffect = hit.get('hit_effect', [{}])[0]
                
                args[0][0] = {
                    'weapon_guid': hit['weapon_guid'],
                    'ammo': hit.get('ammo', False),
                    'spell_id': hit['spell_id'],
                    'level': hit['level'],
                    'caster': hit['caster'],
                    'caster_pos': hit['caster_pos'],
                    'damage_result': {
                        targetEntity.id: {
                            'weapon_id': curWeapon.equip_id,
                            'weapon_guid': curWeapon.guid,
                            'damage': damage,
                            'hit_part': bone['name'],
                            'hit_dir': hitDir,
                            'target_pos': targetEntity.position,
                            'shoot_idx': '',
                            'verify_hit_offset': V32T(hitOffset),
                            'hit_pos': V32T(hitPos),
                            'penetrate_power': 100.0, 'penetrate_materials': [1001, 1],
                            # 'hit_back': True
                        }
                    },
                    'effect': [
                        {
                            'hit_pos': V32T(hitPos),
                            'hit_normal': V32T(raycast.Normal),
                            'hit_dir': hitDir,
                            'hit_material_type': 1001,
                            'weapon_id': curWeapon.equip_id,
                            'effect_id': hitEffect.get('effect_id', 0),
                            'target_id': targetEntity.id
                        }
                    ],
                    'verify_start_pos': V32T(startPos),
                    'extra': {
                        'gun_id': curWeapon.gun_id,
                        'make_ballistick_effect': {
                            'verify_timestamp': time.time() #hit.get('verify_timestamp') or hit.get('extra', {}).get('make_ballistick_effect', {}).get('verify_timestamp') or time.time()
                        }
                    }
                }
                
                # args[0] = [Hit]
                
                # print(f"EXIIT: FORCE({irepr(*args, **kwargs)})")
        
    return self._CallServerGameLogic(method, *args, **kwargs)

    
# from gclient.util.gun_ui_util import GetGunInfoByGunId
# from gshare.weapon_util import GetAllWeaponSkinListByGunID

import random
import os

from gclient.gamesystem.uigunsmith.gunsmith_diy_window import GunSmithDiyWindow
from gclient.gameplay.logic_base.equips.equip_case import EquipCaseFactory

WEP = {
    # AR
    1: ['M4A1', 11199011],
    88: ['SCAR', 231199007, 231199009],
    40: ['AK47', 101100013, 101100008, 101199005, 101100003, 101199009, 101100007, 101199004, 101100002, 101199008, 101100006, 101100001, 101199002, 101100010, 101199006, 101100005, 101199001, 101100032, 101100027, 101100022, 101100017, 101100031, 101100026, 101100021, 101100016, 101100011, 101100006, 101100001, 101100030, 101100025, 101100020, 101100015, 101100010, 101100009, 101199007, 101100005, 101199002, 101100034, 101100029, 101100024, 101100019, 101100014, 101100009, 101199006, 101100004, 101199001, 101100033, 101100028, 101100018, 1011990112, 101199010, 101199011, 1011990100, 1011990101, 1011990102, 1011990110, 1011990111],
    91: ['VSS', 251100016],
    72: ['KAG', 1411990110, 3911990142],
    98: ['AUG]', 291199009],
    93: ['AR97', 271199002, 271100007],
    101: ['MCX', 311199006],
    102: ['FAL', 11100011, 321199007, 321199014],
    115: ['GALIL', 3811990106, 381199003],
    114: ['FN2000', 371199005],
    125: ['RPK', 411199001],
    121: ['QBZ', 391199009],
    108: ['PKM', 341199010],

    # Sniper
    77: ['M700', 181100008, 181100019, 181100018],
    103: ['KAR98', 331100003, 331199011],
    71: ['KALA', 131199005, 131100010, 131199009, 131100014],
    122: ['SVD', 401199001],
    83: ['BOW', 221100008],

    # Shotgun
    34: ['ORIGIN12', 61100011, 61100013],
    79: ['MP155', 201100011],

    # SMG / Pistol
    15: ['GLOCK', 41100015],
    74: ['DEAGLE', 151100011],
    2: ['MP5', 21100002, 21100009],
    38: ['VECTOR', 81199007],
    75: ['URB', 161199008],
    76: ['INP9', 171199006],
    90: ['P90', 241100012, 241100011, 241100013, 241199004, 241100002],
    110: ['UZI', 351199006, 351199005],
    129: ['UMP49', 431199002, 4311990042],
    106: ['PDG', 31000010],
    33: ['Piso', 51000001], 
    95: ['Piso2', 51000001],  
    130: ['KAPAK', 21000010],
    
    # MELE
        33: ['DAGGER', 11000016],
        86: ['KATANA', 31000005], 
        84: ['AXE', 21000005],
}


@HOOK(EquipCaseFactory, 0)
def Create(wid, *args, **kwargs):
    # weaponCase = EquipCaseFactory._Create(wid, *args, **kwargs)
    # GetAllWeaponSkinListByGunID(weaponCase.gun_id)

    args = list(args)

    if sync_attrs.get('bSkinHack'):
        if wid in WEP:
            wids = WEP[wid][1:]
            if wids: args[4] = random.choice(wids)

    result = EquipCaseFactory._Create(wid, *args, **kwargs)
    if sync_attrs.get('bSkinHack'):
        if result:
            result.show_guise_bullet_trace = True
            # result.show_guise_hit_effect = False
    
    return result

@HOOK(GunSmithDiyWindow)
def ChangeGunGuise(self, *_, **__):
    return self._ChangeGunGuise(*_, **__)
    
)srcpy").chr();

