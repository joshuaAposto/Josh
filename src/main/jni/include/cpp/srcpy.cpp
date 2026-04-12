

#include "../obfuscate.h"

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

def IsVisible(local, target, targetId, distance=100):
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

_registered_key = None
_pending_registration = False
_monitor_started = False

def _check_needs_registration():
    global _registered_key, _pending_registration
    try:
        space = getattr(Space, '_instance', None)
        if not space: return
        if not getattr(space, 'entities', None): return

        tick = getattr(StoryTick, '_instance', None)
        if not tick: return

        current_key = (id(space), id(tick))
        if _registered_key != current_key:
            _pending_registration = True
    except Exception:
        pass

def _do_register():
    global _registered_key, _pending_registration
    if not _pending_registration: return
    try:
        space = getattr(Space, '_instance', None)
        tick = getattr(StoryTick, '_instance', None)
        if not space or not tick: return

        for update in REGISTER_UPDATES:
            tick.Add(update, 60)
        _registered_key = (id(space), id(tick))
        _pending_registration = False
        print("[MOD] Entity features registered for current match")
    except Exception:
        print(__import__('traceback').format_exc())

def _entity_monitor():
    while True:
        try:
            _check_needs_registration()
        except Exception:
            pass
        time.sleep(3)

if not _monitor_started:
    _monitor_started = True
    _entity_monitor_thread = threading.Thread(target=_entity_monitor, daemon=True)
    _entity_monitor_thread.start()

@HOOK(StoryTick, 0)
def OnUpdate(self, *args, **kwargs):
    _do_register()
    return self._OnUpdate(*args, **kwargs)

@HOOK(LoginWindow, 0)
def TryAutoEnterGame(self):
    _pending_registration = True
    self.OnEnterGameClick()


import os, struct, json

from gclient.framework.util.story_tick import StoryTick

from gclient.gameplay.logic_base.entities.combat_avatar import CombatAvatar
from gclient.framework.util.replay_util import IsPlayerTeammate

ENTITY_FIFO_PATH = "/data/data/" + PACKAGE_NAME + "/imgui/esync"
if not os.path.exists(ENTITY_FIFO_PATH): os.mkfifo(ENTITY_FIFO_PATH)

def TOT2(x): return x[0], x[1]



@TRY
def EspUpdate(*_, **__):
    inst = genv.space

    if not isinstance(inst, Space): return
    if not inst.entities: return
    if not genv.avatar: return

    localEntity = genv.avatar.combat_avatar
    if not localEntity: return
    if not hasattr(localEntity, 'model'): return
    if sync_attrs.get('bSpeed'): localEntity.model.SetMoveSpeed(sync_attrs.get('fSpeed', 1) + 8)

    cam_pos = Space._instance.camera.position
    entities = []
    for entityId, entity in inst.entities.copy().items():
        if not isinstance(entity, CombatAvatar): continue
        if not getattr(entity, 'model', None): continue
        if not entity.is_alive: continue
        if IsPlayerTeammate(entityId): continue

        if sync_attrs.get('bXray'): entity.model.UseTechHighLightXray(param=(255,0,0), param2=(0,0,255,0), color2=(0,255,0))
        entityHeadPos = entity.model.GetBoneWorldPosition('biped Head')
        if not entityHeadPos: continue

        entityHeadPosx = WorldToScreenPoint(entityHeadPos)
        on_screen = entityHeadPosx.z > 0.0
        dist = Distance(cam_pos, entityHeadPos)

        # skip full bone calculation for far/off-screen entities to reduce CPU load
        if on_screen and dist < sync_attrs.get('fESP_MaxDist', 300):
            def gs(bone): return TOT2(WorldToScreenPoint(entity.model.GetBoneWorldPosition(bone)))
            bones = {
                "head":             gs('biped Head'),
                "spine":            gs('biped Spine'),
                "spine1":           gs('biped Spine1'),
                "limbs_r_upperarm": gs('biped R UpperArm'),
                "limbs_l_upperarm": gs('biped L UpperArm'),
                "limbs_r_forearm":  gs('biped R Forearm'),
                "limbs_l_forearm":  gs('biped L Forearm'),
                "limbs_r_hand":     gs('biped R Hand'),
                "limbs_l_hand":     gs('biped L Hand'),
                "limbs_r_thigh":    gs('biped R Thigh'),
                "limbs_l_thigh":    gs('biped L Thigh'),
                "limbs_r_calf":     gs('biped R Calf'),
                "limbs_l_calf":     gs('biped L Calf'),
                "limbs_r_foot":     gs('biped R Foot'),
                "limbs_l_foot":     gs('biped L Foot'),
                "limbs_r_toe0":     gs('biped R Toe0'),
                "limbs_l_toe0":     gs('biped L Toe0'),
            }
        else:
            head_sc = TOT2(entityHeadPosx)
            bones = {"head": head_sc, "spine": head_sc, "spine1": head_sc,
                     "limbs_r_upperarm": head_sc, "limbs_l_upperarm": head_sc,
                     "limbs_r_forearm": head_sc,  "limbs_l_forearm": head_sc,
                     "limbs_r_hand": head_sc,     "limbs_l_hand": head_sc,
                     "limbs_r_thigh": head_sc,    "limbs_l_thigh": head_sc,
                     "limbs_r_calf": head_sc,     "limbs_l_calf": head_sc,
                     "limbs_r_foot": head_sc,     "limbs_l_foot": head_sc,
                     "limbs_r_toe0": head_sc,     "limbs_l_toe0": head_sc}

        weapon_id = 0
        weapon_name = "None"
        if hasattr(entity, 'GetCurWeapon'):
            try:
                weapon = entity.GetCurWeapon()
                if weapon:
                    weapon_id = weapon.equip_id
                    weapon_name = WEP.get(weapon_id, ["None"])[0]
            except:
                pass

        # only do visibility raycast for on-screen entities within range
        is_vis = IsVisible(cam_pos, entityHeadPos, entity.id) if on_screen and dist < 150 else False

        entities.append({
            "is_on_screen": on_screen,
            "worldpos": [entityHeadPos.x, entityHeadPos.y, entityHeadPos.z],
            "distance": dist,
            "bones": bones,
            "is_bot": entity.IsRobotCombatAvatar,
            "is_knocked": entity.is_dying_state,
            "is_visible": is_vis,
            "is_health": float(getattr(entity, 'hp', 123.0)),
            "is_armor": float(getattr(entity, 'armor', 125.0)),
            "is_team_id": float(getattr(entity, "team", 0)),
            "team_id": int(getattr(entity, "team", 0)),
            "name": getattr(entity, "name", ""),
            "weapon_id": weapon_id,
            "weapon_name": weapon_name,
        })


    with open(ENTITY_FIFO_PATH, 'w') as fifo:
        json.dump(entities, fifo)
        fifo.write('\n')

REGISTER_UPDATES.append(EspUpdate)

from gclient.gameplay.logic_base.entities.combat_avatar import CombatAvatar
from gclient.framework.util.replay_util import IsPlayerTeammate

from gclient.framework.util.gyroscope import GyroscopePoseCombat

from typing import Optional

from gclient.gameplay.logic_base.equips.weapon_case import WeaponCase
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


_aim_tick = 0
@TRY
def AIMUpdate(*_, **__):
    global _aim_tick
    _aim_tick += 1
    if _aim_tick % 2 != 0: return  # run every 2 ticks to halve CPU load
    if not sync_attrs.get('bAIM'): return
    if not genv.avatar: return

    localEntity = genv.avatar.combat_avatar
    if not localEntity: return

    match sync_attrs.get('iAIM_Trigger', 0):
        case 0:
            if not localEntity.is_shooting and not localEntity.is_ads: return
        case 1:
            if not localEntity.is_ads: return
        case 2:
            if not localEntity.is_shooting: return
        case 3:
            pass

    if not FpsPlacerInstance: return

    bone_mapping = {
        0: 'biped Head',       # Head
        1: 'biped Neck',       # Neck
        2: 'biped Spine1',     # Upper Chest
        3: 'biped Spine',      # Lower Chest
        4: 'biped Pelvis',     # Pelvis
        5: 'biped L Hand',     # Left Hand
        6: 'biped R Hand',     # Right Hand
        7: 'biped L Thigh',    # Left Thigh
        8: 'biped R Thigh',    # Right Thigh
        9: 'biped L Foot',     # Left Foot
        10: 'biped R Foot',    # Right Foot
        11: random.choice(['biped Head', 'biped Spine1', 'biped Pelvis']) # Random
    }

    selected_bone = bone_mapping.get(sync_attrs.get('iAIM_TargetBone', 0), 'biped Head')

    targetEntity = None
    closest_distance = sync_attrs.get('fAIM_Fov', 1500)
    screen_center = Vector2(sync_attrs.get('fWidth', 2400) * 0.5, 
                          sync_attrs.get('fHeight', 1080) * 0.5)

    for entityId, entity in Space._instance.entities.copy().items():
        if not isinstance(entity, CombatAvatar): continue
        if not entity.is_alive: continue
        if IsPlayerTeammate(entityId): continue

        bone_pos = entity.model.GetBoneWorldPosition(selected_bone)
        if not bone_pos:
            bone_pos = entity.model.GetBoneWorldPosition('biped Head')
            if not bone_pos: continue

        if sync_attrs.get('bAIM_CheckVisibility', True):
            head_pos = entity.model.GetBoneWorldPosition('biped Head')
            if not head_pos or not IsVisible(Space._instance.camera.position, head_pos, entity.id):
                continue

        screen_pos = WorldToScreenPoint(bone_pos)
        if screen_pos.z <= 0.0: continue

        distance = Distance2D((screen_pos.x, screen_pos.y), 
                            (screen_center.x, screen_center.y))
        
        if distance < closest_distance:
            closest_distance = distance
            targetEntity = entity
            final_screen_pos = screen_pos

    if not targetEntity: return
    if sync_attrs.get('bAIM_IgnoreBots', False) and targetEntity.IsRobotCombatAvatar: return
    if sync_attrs.get('bAIM_IgnoreKnocked', True) and targetEntity.is_dying_state: return

    hitDir = Vector2(
        final_screen_pos.x - screen_center.x,
        final_screen_pos.y - screen_center.y
    )
    
    if abs(hitDir[0]) < 0.00001 and abs(hitDir[1]) < 0.00001: return

    smoothing = 0.00065 * sync_attrs.get('fAIM_SnapStrength', 1.0)
    FpsPlacerInstance.TurnView(
        hitDir[0] * smoothing,
        -hitDir[1] * smoothing
    )

REGISTER_UPDATES.append(AIMUpdate)

from gclient.gameplay.logic_base.entities.combat_avatar import CombatAvatar, PlayerCombatAvatar

BONES = {
    'Head': {
        'name': 'Head',
        'bone_name': 'biped Head',
        'dmg_name': 'head_damage'
    },
    'UpperTop': {
        'name': 'UpperTop',
        'bone_name': 'biped Spine',
        'dmg_name': 'uppertop_damage'
    },
}



from gclient.gameplay.logic_base.entities.cdrone import DRONE_CAMERA_OFFSET
Vector3 = type(DRONE_CAMERA_OFFSET)

def GetHitData(wp, entity, bone):
    startPos = Space._instance.camera.position
    bonePos = entity.model.GetBoneWorldPosition(bone['bone_name'])

    # cap at 40 iterations instead of 150 to reduce CPU usage and overheating
    for _ in range(40):
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
        if IsPlayerTeammate(entityId): continue
        
        entityPosx = WorldToScreenPoint(Vector3(*entity.position))
        if entityPosx.z <= 0.0: continue
        
        # if not IsVisible(Space._instance.camera.position, entity.model.GetBoneWorldPosition('biped Head')): continue
        
        distance = Distance2D((entityPosx.x, entityPosx.y), (sync_attrs.get('fWidth', 2400) * 0.5, sync_attrs.get('fHeight', 1080) * 0.5))
        if distance < nearestDistance:
            nearestDistance = distance
            targetEntity = entity
    
    return targetEntity

import time
from gclient.framework.util.replay_util import IsPlayerTeammate

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
                
                for _, hitData in hit['damage_result'].items():
                    targetId = hitData.get('target_id', None)
                    if hitData.get('hit_part', 'Chest') == 'Chest': continue
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
                    

                    
                    hitData['hit_dir'] = hitDir
                    hitData['hit_pos'] = V32T(hitPos)
                    hitData['hit_part'] = bone['name']
                    hitData['damage'] = damage
                    hitData['verify_hit_offset'] = V32T(hitOffset)
                    
                    for hitEffect in hit.get('hit_effect', ()):
                        hitEffect['hit_pos'] = hitData['hit_pos']
                        hitEffect['hit_normal'] = V32T(Space._instance.RawRaycast(Space._instance.camera.position, 300, 19, with_trigger=False, to_pos=hitPos).Normal)
                        hitEffect['hit_dir'] = hitData['hit_dir']
            
            elif 'ballistic_effect' in hit or ('sound_result' in hit and 'make_ballistick_effect' in hit.get('extra', ())):
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
                

                
                hitEffect = hit.get('hit_effect', [{}])[0]
                
                args[0][0] = {
                    'weapon_guid': hit['weapon_guid'],
                    'cost_ammo': hit.get('cost_ammo', False),
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
                    'hit_effect': [
                        {
                            'hit_pos': V32T(hitPos),
                            'hit_normal': V32T(raycast.Normal),
                            'hit_dir': hitDir,
                            'hit_material_type': 1001,
                            'weapon_id': curWeapon.equip_id,
                            'hit_effect_id': hitEffect.get('hit_effect_id', 1),
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

from gclient.gamesystem.uigunsmith.gunsmith_diy_window import GunSmithDiyWindow
from gclient.gameplay.logic_base.equips.equip_case import EquipCaseFactory

WEP = {
    #AR
1: ['M4A1',
    11199015,  # Black Dragon EVO
    11199016,  # Gold Dragon EVO
    11199017,  # Red Dragon EVO
    11199018,  # Green Dragon EVO
    11199019,  # Ice Dragon EVO
    11199020,  # White Dragon EVO
    11100018,  # Neo Core
    11100020,  # Matrix Prime
],



    88: ['SCAR', 231100022, 231100021, 231100027, 231100029, 231100016, 231199010],
40: ['AK-47',
    101199002,  # AOT Evo Blue
    101100022,  # Quartz EVO
    101100030,  # AOT Evo Red
    101199010,  # Red Evo Mamba
    101199011,  # Blue Evo Mamba
    101100016,
    1011990125
    ],
    72: ['KAG6', 141100011, 141100012, 141100013, 141100014, 3911990132, 3911990142], #141100011 == 10 == 9
    91: ['VSS', 251100016, 251100017, 251100018, 251100019, 251100015, 251100014, 251100013, 251100012, 251100011], #251100016?250100101
    93: ['AR97', 2711990135], #-
    98: ['AUG', 291199012, 291100010, 291199007, 291100005, 2911990091, 291199011, 291100009, 291199006, 291100004, 291199001, 2911990095, 291100008, 291199005, 291100003, 2911990094, 291199014, 291199009, 291100007, 291199004, 291100002, 2911990093, 291199013, 291100011, 291199008, 291100006, 291199003, 291100001, 2911990092, 291199017, 2911990155], #-
    101: ['MCX', 311199005, 311199014, 311199009, 311199004, 311199013, 311199008, 311199003, 311100001, 311199012, 311199007, 311199002, 311199016, 311199011, 311199006, 311199001, 311199015, 311199010], #-
    102: ['FAL', 321199020], #-
    114: ['FN2000', 371199003, 371199002, 371199006, 371199001, 371199005, 371199004, 371199005], #-
    91: ['VSS', 251100014, 251100009, 251100004, 251199001, 251100013, 251100008, 251100003, 251100012, 251100007, 251199004, 251100016, 251100011, 251100006, 251199003, 251100001, 251100015, 251100010, 251100005, 251199002], #-
115: ['GALIL', 3811990034, 3811990142, 3811990105],


    
    
  121: ['QBZ95', 3911990124, 3911990154, 391199012, 3911990106], #-
    
    #SNIPER
    77: ['M700', 181100020, 181100019, 181100018, 181100017, 181100016, 181100015, 181100008], #181100019 == 18 == 20
    103: ['KAR98', 331100003, 331199012, 331199007, 331199002, 331199011, 331199006, 331199001, 331199010, 331199005, 331199009, 331199004, 331100002, 331199008, 331199003, 331100001, 331199014, 331199014], #-
    71: ['KALA', 1311990074, 131100012, 131199009, 131100007, 131199004, 131100002, 1311990073, 131100011, 131199008, 131100006, 131199003, 131100001, 131100010, 131100005, 131199002, 1311990071, 131100014, 131100009, 131199006, 131100004, 1311990075, 1311990070, 131100013, 131100008, 131199005, 131100003, 1311990072, 131199008], #-
    122: ['SVD', 401199002, 401199006, 401199001, 401199005, 401199004, 401199003], #-
    83: ['BOW', 221100005, 221100009, 221100004, 221100008, 221100003, 221100007, 221100002, 221100006, 221100001], #-
    
    #SHOTGUN
    34: ['ORIGIN12', 61100017, 61100012, 61100007, 61199004, 61100002, 61100016, 61100011, 61100006, 61199003, 61100001, 61100015, 61100010, 61100005, 61100011],  #-
    79: ['MP155', 201100012, 201100007, 201199004, 201100002, 201100011, 201100006, 201199003, 201100001, 201100010], #-
    126: ['M1887', 421199001, 421199002], #-
    
    #SMG
    2: ['MP5', 211990193, 21100011, 21100009, 21100008, 21100007, 21100006, 21100005, 21100004, 21100003, 21100002, 21100000, 21199005, 21199016, 211990156, 211990135], # 21199005 == 21100009 red
    129: ['UMP45', 431199002, 4311990032, 4311990042], #-
    38: ['VECTOR', 81199007, 81199008, 81199009, 81199006, 81199005, 81199004, 81199003, 81199002, 81199001], #81199007 great, 81100015 no!
    75: ['URB', 161100011, 161199008, 161100006, 161199003, 161100001, 161100015, 161100010, 161199007, 161100005, 161199002, 161100014, 161100009, 161199006, 161100004, 161199001, 161100013, 161100008, 161199005, 161100003, 161100012, 161100007, 161199004, 161100002, 161199009], #-
    76: ['INP9', 171100013, 171199010, 171100008, 171199005, 171100003, 171199014, 171100012, 171199004, 171100002, 171199013, 171100011, 171100006, 171199003, 171100001, 171100015, 171199012, 171100010, 171100005, 171199002, 171100014, 171199011, 171100009, 171199006, 171100004, 171199001], #-
    90: ['P90', 241100012, 241100011, 241100013, 241100002, 241199014], #241100008, == 10 ==9 == 12 gold 11 nakali
    110: ['UZI',351199006, 351199013, 3511990132, 351199012, 3511990122, 351199005], #-
    #PISTOL
    100: ['MAGNUM', 301199008], #-
    15: ['GLOCK', 41100014, 41100009, 41100004, 41199001, 41100013, 41100008, 41100003, 41100012, 41100007, 41199004, 41100002, 41199002, 41100005, 41100010, 41100015], #-
    74: ['DEAGLE', 151100010, 151100005, 151199002, 151100009, 151100004, 151199001, 151100013, 151100008, 151199005, 151100003, 151100012, 151100007, 151199004, 151100002, 151100011, 151100006, 151199003, 151100001], #-
    
108: ['PKM', 341199008],
131: ['SPEAR', 441199009, 4411990072],
82: ['RPG', ],
138: ['MP7', 481199002],
133: ['M82', 4511990052, 4511990042, 451199004],
125: ["RPK", 411199006],
# MELEE
33: ['KNIFE', 11000008, 11000011, 11000012, 11000016, 11000015],
86: ['KAT4NA', 21000008, 31000007, 31000004, 31000010, 21000007, 31000005],
95: ['KATANA', 51000001],
106: ['BLADE', 31000010, 31000005],
84: ['Axe', 21000004, 21000003, 21000005, 21000007, 21000011],
96: ['Great Axe', 21000005],
97: ['DUAL KNIFE', 11000009, 11000007, 11000013],
107: ['DUAL', 21000007, 21000008, 21000011],
112: ['BLADEZ', 31000007],
112: ['Baseball', 41000002],
117: ['Sword', 31000009],
3: ['GRENADE', ],
4: ['GRENADE', ],
7: ['GRENADE', ],
12: ['GRENADE', ],
81: ['GRENADE', ],
137: ['Kukri', 11000016],
99: ['Shuriken', 11000012],
132: ['Long Knife', 11000015],
132: ['Spear New', 21000009],


}
  
LOG_PATH = "/storage/emulated/0/Android/data/com.netease.newspike/skinlog.txt"

# ===================== [ HOOK EQUIP ] =====================
@HOOK(EquipCaseFactory, 0)
def Create(wid, *args, **kwargs):
    args = list(args)

    try:
        if sync_attrs.get('bSkinHack'):
            if wid in WEP:
                wids = WEP[wid][1:]
                skin_id = sync_attrs.get('iSkin_ID')

                if skin_id and skin_id in wids:
                    if len(args) > 4:
                        args[4] = skin_id
                elif wids:
                    if len(args) > 4:
                        args[4] = random.choice(wids)

    except Exception as e:
        print(f"[Create Hook Error] {e}")

    # --- simpan result dulu ---
    result = EquipCaseFactory._Create(wid, *args, **kwargs)

    # --- aktifkan efek ---
    if result:
        result.show_guise_bullet_trace = True
        result.show_guise_hit_effect = True


    return result


@HOOK(GunSmithDiyWindow)
def ChangeGunGuise(self, *_, **__):
    return self._ChangeGunGuise(*_, **__)



)srcpy").chr();

