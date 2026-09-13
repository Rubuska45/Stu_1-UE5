# Stu_1 — UE5 第三人称 Slash 学习项目

> 自学 Unreal Engine 5（C++ + 蓝图）的练习项目，记录我从零开始的成长轨迹。

## 项目简介
- 引擎版本：UE 5.8
- 类型：第三人称动作（Slash）学习 Demo
- 语言：C++（Visual Studio）+ 蓝图（Blueprint）
- 核心角色类：`SlashCharacter`；主武器类：`AWeapons`

## 学习路线 / 里程碑

> 最近更新：2026-09-13 — 敌人 AI 巡逻与寻路（当前 HEAD `616f550`）。每次上传前会同步更新本文件。

1. **武器拾取与装备**
   - `SlashCharacter` 与 `AWeapons` 通过 Overlap 检测 → 按 `E` 键 pickup / equip 切换 → 挂载到专用武器 Socket。
2. **无武器三段拳击连招 `FnsAttack`**
   - 在 `SlashCharacter` 上复刻剑击 Attack 结构，实现 3 段拳击连招。
   - 蒙太奇 `AM_RoxAttack`：Section `FnsAttack1 / 2 / 3` + `FnsAttackEnd` 通知，Slot `DefaultGroup.Arms`。
3. **武器 Trace 调试（`Weapons.cpp`）**
   - 围绕 `BoxTraceStart` / `BoxTraceEnd` 两个 SceneComponent 排查 trace 起点/终点与命中逻辑。
   - 屏蔽自身命中：用 `GetAttachParentActor()` 忽略武器持有者实例（而非按通道/类一刀切）。
4. **敌人受击反馈系统**（`899cd61` `61c4049` `37732af`）
   - 实现 `IHitInterface` 受击接口：被武器命中时按攻击来源方向做受击反应（方向反应），而非固定倒地。
   - 修复「一次攻击多次碰撞」：同一挥击对同个目标只生效一次（防连段重复扣血 / 重复触发特效）。
   - 受击 / 武器攻击特效：方向反应动画 + 音效 + Niagara 粒子反馈。
5. **可破坏罐子与金币战利品**（`29173d3` `9cee810` `b7c19f7`）
   - 武器命中时触发蓝图可实现的物理场事件，打通「击碎罐子」链路。
   - 罐子用 Chaos 几何集合 + 物理场实现可破坏效果。
   - 罐子被击中随机掉落金币 / 财宝，每个战利品可配置独立价值；金币可被玩家拾取。
6. **敌人血条与死亡系统**（`62afc32` `e40d865` `14b32dd`）
   - `HealthBarComponent` + 属性组件：敌人血条 UI 与血量属性解耦。
   - 血量实时同步显示；随机死亡动画（`EDeathPose` 枚举 + `Blend Poses by Enum`）。
   - 死亡姿势保持：尸体约 3 秒后销毁；血条随死亡状态显隐控制。
7. **敌人 AI 巡逻与寻路（本次）**（`1b0ce16` `616f550`）
   - `UPawnSensingComponent` 感知玩家：进入视锥即切 `EES_Chasing`，调用 AI 控制器 `MoveTo` 寻路追击。
   - 玩家跑出战斗半径（CombatRadius）自动回到 `EES_Patrolling` 巡逻；进入 `AttackRadius` 即攻击。
   - 受击时锁定攻击者并追逐。
   - 状态机 `EEnemyState`（Patrolling / Chasing / Attacking）驱动 `Tick` 分支逻辑，`AddDynamic` 订阅 `OnSeePawn` 事件。

## 仓库结构
| 路径 | 说明 | 是否跟踪 |
|------|------|----------|
| `Source/` | C++ 源码（.h / .cpp / .cs） | 是 |
| `Config/` | 项目配置（.ini） | 是 |
| `Stu_1.uproject` | 项目描述文件 | 是 |
| `README.md` | 本说明 | 是 |
| `Content/` | 蓝图与资源（约 3.7GB） | 否（超免费 LFS 额度，本地保留） |
| `Binaries/` `Intermediate/` `Saved/` `DerivedDataCache/` | 编译缓存 / 可重建产物 | 否 |

> 说明：为保证仓库轻量、可免费托管，编译缓存与全部 `Content` 资源均不纳入版本控制。本仓库专注于记录 C++ 代码与学习笔记。若需完整可运行工程，请在本机保留 `Content/` 后打开 `.uproject`。

## 如何运行（本机）
1. 安装 UE 5.8 与 Visual Studio（含 UE C++ 工作负载）。
2. 克隆源码：`git clone https://github.com/Rubuska45/Stu_1-UE5.git`
3. 将你本机的 `Content/` 放入项目根目录（与仓库源码合并）。
4. 用 UE 打开 `Stu_1.uproject`，等待编译 C++ 模块即可运行。

## 提交约定
- 每次完成功能 / 修复：**先更新本 README 的「学习路线 / 里程碑」**，再 `git add Source Config Stu_1.uproject README.md` → `git commit -m "中文说明"` → `git push`。
- `Content` 资源不纳入仓库，请在本机另存备份。

## 声明
学习用途，代码与思路供交流参考。
