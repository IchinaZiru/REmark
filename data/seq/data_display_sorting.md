sequenceDiagram
    User->>Frontend: 予約表示/ソート機能を選択
    alt 全ての予約を日付でソート表示 fun-029
        Frontend->>Backend: GET /api/reservations?sortBy=date&sortOrder=asc
        Backend->>DB: 全予約情報を取得
        DB-->>Backend: 予約データ
        Backend->>Backend: 予約データを日付でソート
        Backend-->>Frontend: ソート済み予約リスト
        Frontend-->>User: 日付でソートされた予約リストを表示
    else 特定ユーザーの予約表示 fun-030
        User->>Frontend: ユーザーIDを入力
        Frontend->>Backend: GET /api/reservations?userId={ユーザーID}
        Backend->>DB: 特定ユーザーの予約情報を取得
        DB-->>Backend: 予約データ
        Backend-->>Frontend: 該当ユーザーの予約リスト
またはメッセージ
        Frontend-->>User: 予約リスト
またはメッセージを表示
    else 特定会議室の予約表示 fun-031
        User->>Frontend: 会議室IDを入力
        Frontend->>Backend: GET /api/reservations?roomId={会議室ID}
        Backend->>DB: 特定会議室の予約情報を取得
        DB-->>Backend: 予約データ
        Backend-->>Frontend: 該当会議室の予約リスト
またはメッセージ
        Frontend-->>User: 予約リスト
またはメッセージを表示
    else 予約期間指定検索表示 fun-032
        User->>Frontend: 開始日と終了日を入力
        Frontend->>Backend: GET /api/reservations?startDate={開始日}&endDate={終了日}
        Backend->>DB: 指定期間内の予約情報を取得
        DB-->>Backend: 予約データ
        Backend-->>Frontend: 指定期間内の予約リスト
またはメッセージ
        Frontend-->>User: 予約リスト
またはメッセージを表示
    end