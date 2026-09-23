# GPlan — Agent 速查

## 构建与运行
- **引擎**：Unreal Engine 5.5（`.uproject` 中 `"EngineAssociation": "5.5"`）
- **打开编辑器**：双击 `GPlan.uproject`
- **IDE 编译**：打开 `GPlan.sln` → 编译 `GPlan`（Game）或 `GPlanEditor`（Editor）
- **Live Coding**：可用（见 `.cursor/rules/unreal-live-coding.mdc`）；增删 `UPROPERTY` / `UFUNCTION` / `UCLASS` 等反射项需完整编译

## 模块结构
- 单一 Runtime 模块：`Source/GPlan/`（`GPlan.Build.cs`）；源码扁平放置，无 `Public/`、`Private/` 拆分
- Target：`GPlan.Target.cs`（Game）、`GPlanEditor.Target.cs`（Editor）；`BuildSettingsVersion.V5`，`IncludeOrderVersion.Unreal5_5`
- `PCHUsage = UseExplicitOrSharedPCHs`
- 当前 Public 依赖：`Core`、`CoreUObject`、`Engine`、`InputCore`、`EnhancedInput`
- `.uproject` 已启用插件：`GameplayAbilities`、`ModelingToolsEditorMode`（仅 Editor）
- **注意**：`GameplayAbilities` 尚未写入 `GPlan.Build.cs`；真正使用 GAS API 前必须先加模块依赖，再写代码

## 核心约定（详见 `.cursor/rules/project-conventions.mdc`）
| 项 | 约定 |
|------|------|
| **类型前缀** | `GP` + UE 前缀 → `AGPPlayerCharacter`、`UGPHealthComponent` |
| **文件名** | 与主类同名（`GPHealthComponent.h/.cpp`） |
| **API 宏** | 跨模块导出的 `UCLASS` 使用 `GPLAN_API` |
| **bool** | `b` 前缀（`bIsDead`） |
| **输入回调** | `Input_` 前缀（`Input_Move`、`Input_Jump`） |
| **动态委托** | `FOn…Sign`（`FOnHPChangedSign`、`FOnDeathSign`） |
| **UPROPERTY Category** | 简短英文（`Camera`、`Input`、`HP`、`Bullet`） |
| **日志** | 仅用 `LogGP`（`UE_LOG(LogGP, Display, ...)`） |

## C++ / UE 习惯
- Header：`#pragma once` → include → 前向声明 → `*.generated.h`（必须最后）
- 实现文件先 include 对应 `.h`；构造函数用 `CreateDefaultSubobject` + `SetupAttachment`
- 子对象：`VisibleAnywhere, BlueprintReadOnly` + `meta=(AllowPrivateAccess="true")`（私有/受限成员）
- 可调数值：`EditDefaultsOnly`；只读状态：`VisibleAnywhere` + `BlueprintReadOnly`
- 不需要每帧逻辑时关 Tick（组件：`PrimaryComponentTick.bCanEverTick = false`）
- Override 先 `Super::`；指针判空；数值做边界保护（如血量不为负）
- 对外事件：`DECLARE_DYNAMIC_MULTICAST_DELEGATE_*` + `BlueprintAssignable`
- 网格 / 动画蓝图等资产在 Blueprint 指定，C++ 不硬编码 `/Game/...` 路径

## 架构职责
- `AGPGameMode`：模式入口
- `AGPPlayerCharacter`：Pawn / 相机 / 移动（**不**绑主输入）
- `AGPPlayerController`：Enhanced Input 绑定与 `Input_*` 回调
- `UGPHealthComponent`：血量与伤害事件
- `AGPProjectile`：投射物

## 新增依赖
1. 改 `Source/GPlan/GPlan.Build.cs`（`PublicDependencyModuleNames` / `PrivateDependencyModuleNames`）
2. 若是插件，同步写入 `GPlan.uproject` → `Plugins`
3. 重新生成工程文件（右键 `.uproject` → Generate Visual Studio project files）

## 禁止事项
- 不要改用非 `GP` 的项目类型前缀，或拆散 `GPXxx` 文件命名
- 不要在 Character 上新增主输入绑定（应扩展 `AGPPlayerController`）
- 不要无必要开 Tick；不要省略 `Super::`；不要跳过空指针检查
- 不要在 C++ 中大面积硬编码 Content 路径；不要引入未写入 `Build.cs` 的模块
- 不要擅自新增平行日志类别替代 `LogGP`；不要提交无关格式化或与任务无关的改动

## Git Ignore（摘要）
- 忽略：`Binaries/`、`Intermediate/`、`Saved/`、`DerivedDataCache/`、`Build/`、`.vs/`、`*.sln`、`*.suo`、`*.pch`、`*.dll`、`*.exe`
- 纳入版本控制：`Source/`、`Content/`、`Config/`、`.uproject` 及 `Source` 下的 `.Build.cs` / `.Target.cs`

## 相关文件
- `.cursor/rules/project-conventions.mdc` — 本项目约定（优先遵循）
- `.cursor/rules/unreal-conventions.mdc` — 通用 UE 命名与风格
- `.cursor/rules/unreal-build-system.mdc` — 通用 Build.cs / Target.cs 说明（其中 Public/Private 拆分、`V4` 示例**不适用于**本仓库；以本文件与 `project-conventions.mdc` 为准）
- `Config/DefaultEngine.ini`、`DefaultGame.ini`、`DefaultInput.ini` — 项目配置
