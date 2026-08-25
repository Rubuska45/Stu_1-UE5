# Stu_1 — UE5 第三人称 Slash 学习项目

> 自学 Unreal Engine 5（C++ + 蓝图）的练习项目，记录我从零开始的成长轨迹。

## 项目简介
- 引擎版本：UE 5.8
- 类型：第三人称动作（Slash）学习 Demo
- 语言：C++（Visual Studio）+ 蓝图（Blueprint）
- 核心角色类：`SlashCharacter`；主武器类：`AWeapons`

## 学习路线 / 里程碑
1. **武器拾取与装备**
   - `SlashCharacter` 与 `AWeapons` 通过 Overlap 检测 → 按 `E` 键 pickup / equip 切换 → 挂载到专用武器 Socket。
2. **无武器三段拳击连招 `FnsAttack`**
   - 在 `SlashCharacter` 上复刻剑击 Attack 结构，实现 3 段拳击连招。
   - 蒙太奇 `AM_RoxAttack`：Section `FnsAttack1 / 2 / 3` + `FnsAttackEnd` 通知，Slot `DefaultGroup.Arms`。
3. **武器 Trace 调试（`Weapons.cpp`）**
   - 围绕 `BoxTraceStart` / `BoxTraceEnd` 两个 SceneComponent 排查 trace 起点/终点与命中逻辑。
   - 屏蔽自身命中：用 `GetAttachParentActor()` 忽略武器持有者实例（而非按通道/类一刀切）。

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
- 每次完成功能 / 修复：`git add Source Config` → `git commit -m "中文说明"` → `git push`
- `Content` 资源不纳入仓库，请在本机另存备份。

## 声明
学习用途，代码与思路供交流参考。
