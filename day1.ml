open! Core

let max_excl = 100

let apply_move_part1 acc dir move_amount =
  let op = match dir with `L -> ( - ) | `R -> ( + ) in
  (op acc move_amount + max_excl) mod max_excl

let solve_part1 moves =
  List.fold_left
    ~f:(fun (acc_v, zero_count) (dir, move_amount) ->
      (* printf "Applying move: %s%d\n"
        (match dir with `L -> "L" | `R -> "R")
        move_amount; *)
      let acc_v = apply_move_part1 acc_v dir move_amount in
      (* printf "Current value: %d\n" acc_v; *)
      (acc_v, if acc_v = 0 then zero_count + 1 else zero_count))
    ~init:(50, 0) moves

let count_zero_hits acc dir move_amount =
  let op = match dir with `L -> ( - ) | `R -> ( + ) in
  let apply_once v = (op v 1 + max_excl) mod max_excl in
  let rec aux (acc_v, acc_zeros) rem =
    if rem = 0 then (acc_v, acc_zeros)
    else
      let next_v = apply_once acc_v in
      aux (next_v, if next_v = 0 then acc_zeros + 1 else acc_zeros) (rem - 1)
  in
  aux (acc, 0) move_amount

let solve_part2 moves =
  List.fold_left
    ~f:(fun (acc_v, zero_count) (dir, move_amount) ->
      let acc_v, added_zeros = count_zero_hits acc_v dir move_amount in
      (acc_v, zero_count + added_zeros))
    ~init:(50, 0) moves

let parse_moves input =
  let parse_move move_string =
    let dir =
      move_string.[0] |> function
      | 'L' -> `L
      | 'R' -> `R
      | _ -> failwith "Unknown direction"
    in
    let amount =
      String.sub move_string ~pos:1 ~len:(String.length move_string - 1)
      |> int_of_string
    in
    (dir, amount)
  in
  String.split ~on:'\n' input |> List.map ~f:parse_move

let () =
  let moves = parse_moves (In_channel.input_all In_channel.stdin) in
  (* let _, zero_count = solve_part1 moves in
  ignore solve_part2; *)
  ignore solve_part1;
  let _, zero_count = solve_part2 moves in
  Printf.printf "Zero count: %d\n" zero_count
