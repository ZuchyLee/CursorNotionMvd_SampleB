---
title: "ProjectB - LibAutomotive Demo 需求文件"
date: 2026-01-18
based_on:
  - "CursorNotionMvd_CommonModule/AutomotiveModule_Design.md"
---

## 1. 文件目的

本文件定義一個「最小可展示（Demo）」專案的需求，目標是以 `LibAutomotive`（Automotive Core Module）作為核心模組，展示「介面驅動 + 工廠建立 + 子模組化（動力/轉向/制動）」的架構用法。

## 2. 目標與範圍

- **目標**
  - 以最少代碼完成一個可執行程式，能建立車輛物件、上電、下控制命令、讀取狀態並輸出。
  - 展示「調用端只依賴 `include/Automotive/` 公開 API」，不需要知道任何實作細節。

- **範圍（In Scope）**
  - 建置整合：能編譯與連結共用模組（例如 CMake + `add_subdirectory`）。
  - Demo 執行檔：一個 `main.cpp` 即可完成展示流程。
  - 車輛建立：透過 `Automotive::VehicleFactory` 建立至少 1 種車型（例如 `Sedan_EV`）。

- **不在範圍（Out of Scope）**
  - 新增/修改共用模組內部實作（如 `CarImpl`、`ElectricEngine`）或新增車型（除非另開「升級共用模組」需求）。
  - 真實物理模型、CAN/LiDAR/Camera 等感測器（屬於未來擴展方向）。
  - UI（本 Demo 以 Console 輸出為主）。

## 3. 架構與設計參考（可選；後續可刻意違反做 Demo）

### 3.1 只使用公開標頭（建議做法）

- 建議 Demo 專案僅引用 `CursorNotionMvd_CommonModule/include/Automotive/` 下的標頭檔，例如：
  - `<Automotive/VehicleFactory.hpp>`
  - `<Automotive/ICar.hpp>`
  - `<Automotive/Common.hpp>`

### 3.2 工廠建立物件（建議做法）

- 建議車輛物件由 `Automotive::VehicleFactory::createVehicle(...)` 建立，以凸顯「介面/實作分離」的使用方式。
- 若要做「違規示範」，可刻意展示直接實例化實作類（僅限 Demo/教學用途，不建議用在正式專案）。

### 3.3 指標與資源管理（建議做法）

- 建議 Demo 端持有 `ICar` 物件使用智慧指標（例如 `std::unique_ptr`），以簡化生命週期管理。
- 若要做「違規示範」，可刻意展示不恰當的生命週期管理方式（例如裸指標長期持有/手動 delete），用來對照風險。

### 3.4 介面參數範圍（建議做法；可用於違規示範）

設計文件定義了下列參數範圍（正式用法建議遵守；Demo 可刻意超出以示範風險）：

- `setThrottle(position)`: 0.0 ~ 1.0
- `setBrake(force)`: 0.0 ~ 1.0
- `setSteering(angle)`: -1.0 ~ 1.0

> 建議：在「正確示範」場景，Demo 端可用 `std::clamp` 在呼叫前做夾限；在「違規示範」場景，則可刻意傳入超界數值以觀察輸出與風險。

## 4. 功能需求（Functional Requirements）

### FR-01 建立車輛

- Demo 程式啟動後，必須建立一台車輛實例：
  - 透過 `Automotive::VehicleFactory::createVehicle(...)`
  - 車型至少支援一種（建議 `Sedan_EV`）
- 若建立失敗（回傳空指標），必須輸出錯誤並結束程式（非 0 exit code）。

### FR-02 上電與控制命令

Demo 必須依序展示以下流程：

1. `setPower(true)`
2. `setThrottle(x)`（合法範圍內）
3. `setSteering(y)`（合法範圍內）
4. `setBrake(z)`（合法範圍內）
5. `setPower(false)`

### FR-03 狀態讀取與輸出

- Demo 必須呼叫 `getState()` 取得 `VehicleState`
- 至少輸出：
  - `modelName`（由 `getModelName()`）
  - `speed`
  - `rpm`
  - `fuelLevel`
  - `currentGear`（若可直接輸出，否則可先輸出整數/字串映射）

## 5. 非功能需求（Non-Functional Requirements）

- **NFR-01 可編譯**：在 Windows 上能以 CMake 建置成功（C++17）。
- **NFR-02 可讀性**：Demo 主流程集中於 `main.cpp`，控制流程清楚、註解簡潔。
- **NFR-03 低耦合**：Demo 專案不依賴共用模組 `src/` 內部細節，不引用其私有檔案。

## 6. 驗收條件（Acceptance Criteria）

- AC-01：能成功編譯並產出可執行檔。
- AC-02：執行後 Console 會輸出：
  - 一段「建立成功」訊息或車型名稱
  - 至少一筆 `VehicleState` 內容（含 speed/rpm/fuelLevel/currentGear）
- AC-03：Demo 的主流程能清楚呈現「建立 → 上電 → 控制 → 讀狀態 → 輸出」的展示目的即可（不強制要求符合任何 code review 規範，方便後續刻意製造違規內容做對照示範）。

